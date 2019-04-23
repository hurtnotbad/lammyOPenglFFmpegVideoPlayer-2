//
// Created by lammy on 2019/4/8.
//

#include <Log.h>
#include <cstdio>
#include "OpenSLESAudioPlayer.h"


#include "SLES/OpenSLES.h"
#include "SLES/OpenSLES_Android.h"
//SLObjectItf 本身就是指针typedef const struct SLObjectItf_ * const * SLObjectItf;
static SLObjectItf engineSL = NULL;
static SLEngineItf eng = NULL;
static SLObjectItf mix = NULL;
static SLObjectItf player = NULL;
static SLPlayItf iplayer = NULL;
static SLAndroidSimpleBufferQueueItf pcmQue = NULL;

// 创建引擎
static SLEngineItf CreateSL()
{
    SLresult re;
    SLEngineItf en;
    re = slCreateEngine(&engineSL,0,0,0,0,0);
    if(re != SL_RESULT_SUCCESS) return NULL;
    re = (*engineSL)->Realize(engineSL,SL_BOOLEAN_FALSE);
    if(re != SL_RESULT_SUCCESS) return NULL;
    re = (*engineSL)->GetInterface(engineSL,SL_IID_ENGINE,&en);
    if(re != SL_RESULT_SUCCESS) return NULL;
    return en;
}

void OpenSLESAudioPlayer::PlayCall(void *bufq)
{
    if(!bufq)return;
    SLAndroidSimpleBufferQueueItf bf = (SLAndroidSimpleBufferQueueItf)bufq;
    //阻塞

    getAudioData();
    (*bf)->Enqueue(bf,buf,dataManager->audioDateSize);


}
static void PcmCall(SLAndroidSimpleBufferQueueItf bf,void *contex)
{
    OpenSLESAudioPlayer *ap = (OpenSLESAudioPlayer *)contex;
    if(!ap)
    {
        return;
    }
    ap->PlayCall((void *)bf);
}


bool OpenSLESAudioPlayer::playAudio(){

    Close();
    dataManager->slesLock.lock();
    //1 创建引擎
    eng = CreateSL();
    if(eng)
    {
        LOGI("CreateSL success！ ");
    }
    else
    {
        dataManager->slesLock.unlock();
        LOGE("CreateSL failed！ ");
        return false;
    }

    //2 创建混音器

    SLresult re = 0;
    re = (*eng)->CreateOutputMix(eng,&mix,0,0,0);
    if(re !=SL_RESULT_SUCCESS )
    {
        dataManager->slesLock.unlock();
        LOGE("SL_RESULT_SUCCESS failed!");
        return false;
    }
    re = (*mix)->Realize(mix,SL_BOOLEAN_FALSE);
    if(re !=SL_RESULT_SUCCESS )
    {
        dataManager->slesLock.unlock();
        LOGE("(*mix)->Realize failed!");
        return false;
    }
    SLDataLocator_OutputMix outmix = {SL_DATALOCATOR_OUTPUTMIX,mix};
    SLDataSink audioSink= {&outmix,0};

    //3 配置音频信息
    //缓冲队列
    SLDataLocator_AndroidSimpleBufferQueue que = {SL_DATALOCATOR_ANDROIDSIMPLEBUFFERQUEUE,10};
    //音频格式
    SLDataFormat_PCM pcm = {
            SL_DATAFORMAT_PCM,
            (SLuint32) 2,//    声道数
            (SLuint32) SL_SAMPLINGRATE_44_1,
            SL_PCMSAMPLEFORMAT_FIXED_16,
            SL_PCMSAMPLEFORMAT_FIXED_16,
            SL_SPEAKER_FRONT_LEFT|SL_SPEAKER_FRONT_RIGHT,
            SL_BYTEORDER_LITTLEENDIAN //字节序，小端
    };
    SLDataSource ds = {&que,&pcm};


    //4 创建播放器
    const SLInterfaceID ids[] = {SL_IID_BUFFERQUEUE};
    const SLboolean req[] = {SL_BOOLEAN_TRUE};
    re = (*eng)->CreateAudioPlayer(eng,&player,&ds,&audioSink,sizeof(ids)/sizeof(SLInterfaceID),ids,req);
    if(re !=SL_RESULT_SUCCESS )
    {
        dataManager->slesLock.unlock();
        LOGE("CreateAudioPlayer failed!");
        return false;
    } else{
        LOGI("CreateAudioPlayer success!");
    }
    (*player)->Realize(player,SL_BOOLEAN_FALSE);
    //获取player接口
    re = (*player)->GetInterface(player,SL_IID_PLAY,&iplayer);
    if(re !=SL_RESULT_SUCCESS )
    {
        dataManager->slesLock.unlock();
        LOGE("GetInterface SL_IID_PLAY failed!");
        return false;
    }
    re = (*player)->GetInterface(player,SL_IID_BUFFERQUEUE,&pcmQue);
    if(re !=SL_RESULT_SUCCESS )
    {
        dataManager->slesLock.unlock();
        LOGE("GetInterface SL_IID_BUFFERQUEUE failed!");
        return false;
    }

    //设置回调函数，播放队列空调用
    (*pcmQue)->RegisterCallback(pcmQue,PcmCall,this);

    //设置为播放状态
    (*iplayer)->SetPlayState(iplayer,SL_PLAYSTATE_PLAYING);

    //启动队列回调
    (*pcmQue)->Enqueue(pcmQue,"",1);
    LOGI("SLAudioPlay::StartPlay success!");
    dataManager->slesLock.unlock();
    return true;

}

OpenSLESAudioPlayer::OpenSLESAudioPlayer(DataManager *dataManager)
{
    this->dataManager = dataManager;
    buf = new unsigned char[1024*1024];
}

void OpenSLESAudioPlayer::getAudioData()
{
    while ((dataManager->isPause)&&!dataManager->isExit){//
//        LOGE("音频暂停中。。。。。。。。");
        LSleep(dataManager->PauseSleepTime);
        continue;
    }

    char *data = nullptr;
    while (!dataManager->isExit) {
        dataManager->audioLock.lock();
        if (dataManager->audioData.size() > 0 &&  dataManager->audioPts.size()>0) {
            data = (char *) (dataManager->audioData.front());
            dataManager->currentAudioPts = dataManager->audioPts.front();
            dataManager->audioPts.pop_front();
            dataManager->audioData.pop_front();
            memcpy(buf,data,dataManager->audioDateSize);
            free(data);
            dataManager->audioLock.unlock();
            return ;
        }
        LOGE("没有数据了，等等");
        dataManager->audioLock.unlock();
        LSleep(2);
        continue;
    }

}
void OpenSLESAudioPlayer::Close()
{
    dataManager->slesLock.lock();
    //停止播放
    if(iplayer && (*iplayer))
    {
        (*iplayer)->SetPlayState(iplayer,SL_PLAYSTATE_STOPPED);
    }
    //清理播放队列
    if(pcmQue && (*pcmQue))
    {
        (*pcmQue)->Clear(pcmQue);
    }
    //销毁player对象
    if(player && (*player))
    {
        (*player)->Destroy(player);
    }
    //销毁混音器
    if(mix && (*mix))
    {
        (*mix)->Destroy(mix);
    }

    //销毁播放引擎
    if(engineSL && (*engineSL))
    {
        (*engineSL)->Destroy(engineSL);
    }

    engineSL = NULL;
    eng = NULL;
    mix = NULL;
    player = NULL;
    iplayer = NULL;
    pcmQue = NULL;
    LOGE("close sles success");
    dataManager->slesLock.unlock();
}
