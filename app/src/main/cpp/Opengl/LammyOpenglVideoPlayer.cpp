//
// Created by lammy on 2019/4/13.
//

#include "LammyOpenglVideoPlayer.h"
#include "FFMResample.h"
#include <thread>
#include <LammyOpenglVideoPlayer.h>
#include <DataManager.h>
#include <FFMDemux.h>
#include <FFMdecode.h>
#include <FFMVideoShow.h>
#include <OpenSLESAudioPlayer.h>

LammyOpenglVideoPlayer::LammyOpenglVideoPlayer()
{
    dataManager = new DataManager();
    ffmDemux= new FFMDemux(dataManager);
    ffMdecode = new FFMdecode(dataManager);
    openglVideoShow = new OpenglShow(dataManager);
    ffmResample = new FFMResample(dataManager);
    openSLESAudioPlayer = new OpenSLESAudioPlayer(dataManager);

}


void LammyOpenglVideoPlayer::videoThreadMain()
{
    initView(dataManager->win);
   while(!dataManager->isExit)
    {

        LOGI(" video thread  .....");
          while(dataManager->videoPauseReady){
            LSleep(dataManager->PauseSleepTime);
            if(dataManager->isExit)
            {
                dataManager->isVideoRunning = false;
                return;
            }
            LOGI("pause videoPauseReady  .....");
//            continue;
        }
        LOGI(" video thread  .....1111");
        AVFrame *  avFrame = ffMdecode->decode(0);
        LOGI(" video thread  .....222");
        if(avFrame != 0 && avFrame != nullptr){
          LOGI("avFrame video show  .....");
            openglVideoShow->show(avFrame);
        }
        else if(avFrame == 0){
            LSleep(2);
        }
        LOGI(" video thread  ....333.");
        if(dataManager->isPausing)//&&dataManager->demuxPauseReady
        {
            LOGI("pause videoPauseReady  .open....");
            dataManager->videoPauseReady = true;
        }

    }
        LOGI(" video thread stop  success.....");
        dataManager->isVideoRunning = false;

}

void LammyOpenglVideoPlayer::clearOpengl()
{

    glDeleteTextures(3,openglVideoShow->glProgram->yuvTexture);
    openglVideoShow->glProgram->yuvTexture[0] = 0;
    openglVideoShow->glProgram->yuvTexture[1] = 0;
    openglVideoShow->glProgram->yuvTexture[2] = 0;
    glDeleteProgram(openglVideoShow->glProgram->program);
    openglVideoShow->glProgram->program = 0;
}

void LammyOpenglVideoPlayer::audioThreadMain()
{
    while(!dataManager->isExit)
    {
        while(dataManager->audioPauseReady)
        {
            LSleep(dataManager->PauseSleepTime);
            if(dataManager->isExit)
            {
                dataManager->isAudioRunning= false;
                return;
            }
        }
        /********************* 解码重采样部分****************************/
        AVFrame * avFrame = ffMdecode->decode(1);
        if(avFrame != 0 && avFrame != nullptr)
        {
//            LOGI("pause resample  ..........");
            ffmResample->resample(avFrame);
        }else{
            LSleep(2);
        }
        /********************* 解码重采样部分结束****************************/

        if(dataManager->isPausing)// && dataManager->videoPauseReady
        {
            LOGI("pause audioPauseReady  .open....");
            dataManager->audioPauseReady = true;
        }

    }
    dataManager->isAudioRunning = false;
    LOGI(" audio thread stop  success.....");

}

void LammyOpenglVideoPlayer::demuxThreadMain()
{
    LOGI("demuxThreadMain  start....");
    while(!dataManager->isExit)
    {
        LOGI("demuxThreadMain  start..1111..");
        while(dataManager->demuxPauseReady)
        {
            LSleep(dataManager->PauseSleepTime);
            if(dataManager->isExit)
            {
                dataManager->isDemuxRunning= false;
                return;
            }
        }
        LOGI("demuxThreadMain  start..22..");
        /********************* 解封装部分****************************/
        int mode = ffmDemux->demux();
        /********************* 解封装部分结束****************************/
        LOGI("demuxThreadMain  start..33..");
        if(dataManager->isPausing)
        {
            LOGI("pause demuxPauseReady  .open....");
            dataManager->demuxPauseReady = true;
        }

    }
    LOGI(" demux thread stop  success.....");
    dataManager->isDemuxRunning= false;

}


void LammyOpenglVideoPlayer::start(const char * videoPath)
{

    if(dataManager -> avFormatContext != nullptr)
    {
        close();
    }
    dataManager->url = videoPath;
    startThread();

}

void LammyOpenglVideoPlayer::close()
{
    if(dataManager->avFormatContext == nullptr)
        return;

    stopThread();
    dataManager->clearData();
    //clearOpengl();


    LOGE("player close success 4 ");
    openSLESAudioPlayer->Close();
    LOGE("player close success 5 ");
    dataManager->closeFFMpeg();
    openglVideoShow->freeSpace();
    LOGE("player close success ");

}

bool LammyOpenglVideoPlayer::stopThread()
{
    dataManager->isExit = true;
    for(int i = 0 ; i < 200; i++)
    {
        if(!dataManager->isVideoRunning&& !dataManager->isDemuxRunning && !dataManager->isAudioRunning){
            openSLESAudioPlayer->Close();
            LEGL::Get()->Close();
            clearOpengl();
            LOGE("stop thread success !");
            return true;
        }
        LSleep(1);
    }
    LOGE("stop time out dataManager->isVideoRunning = %d" ,dataManager->isVideoRunning );
    LOGE("stop time out dataManager->isVideoRunning = %d" ,dataManager->isDemuxRunning );
    LOGE("stop time out dataManager->isVideoRunning = %d" ,dataManager->isAudioRunning );
    LOGE("stop time out !");
    return false;

}



void LammyOpenglVideoPlayer::pauseOrContinue()
{
    if(!dataManager->isPause)
    {
        dataManager->isPausing = true;
        while(true)
        {
            if( dataManager->videoPauseReady &&dataManager->audioPauseReady&&dataManager->demuxPauseReady )//&&dataManager->demuxPauseReady
            {
                dataManager->isPause =true;
                dataManager->isPausing =false;
                // 只有 取消暂停的时候才能 将下面置为true
//                dataManager->videoPauseReady =false;
//                dataManager->audioPauseReady =false;
//                dataManager->demuxPauseReady=false;
                LOGE("pause success");
                return;
            }else{
                LSleep(20);
                continue;
            }
        }
    }
    else
    {
        dataManager->isPause =false;
        dataManager->isPausing =false;
        dataManager->videoPauseReady =false;
        dataManager->audioPauseReady =false;
        dataManager->demuxPauseReady=false;
        LOGE("un pause success");
        return;

    }

}



float progress = 0;
void LammyOpenglVideoPlayer::seekTo(float seekPos)
{
    LOGE("seekPos = %f", seekPos);
    dataManager->seekLock.lock();
    if (dataManager->isSeeking){
        progress = seekPos;
        LOGE(" progress = seekPos = %f", seekPos);
        dataManager->seekLock.unlock();
        return;
    }else{
        progress = seekPos;
        std::thread seek_th(&LammyOpenglVideoPlayer::seekThreadMain,this);
        seek_th.detach();
    }
    dataManager->seekLock.unlock();

}

void LammyOpenglVideoPlayer::seekThreadMain()
{
    dataManager->isSeeking = true;
    if(!dataManager-> isPause)
    {
        pauseOrContinue();
    }

    dataManager->clearData();

    dataManager->seekLock.lock();
    long long pos2 = dataManager->avFormatContext->streams[dataManager->videoStreamIndex]->duration* progress;
    av_seek_frame(dataManager->avFormatContext, dataManager->videoStreamIndex, pos2, AVSEEK_FLAG_FRAME|AVSEEK_FLAG_BACKWARD);
    ffmDemux->seekTo(progress);
    dataManager->currentAudioPts =LLONG_MAX;
    pauseOrContinue();

    dataManager->isSeeking = false;
    dataManager->seekLock.unlock();

}

void LammyOpenglVideoPlayer::OpenSLESThreadMain()
{
    openSLESAudioPlayer->playAudio();
}



void LammyOpenglVideoPlayer::startThread()
{
    dataManager->isAudioRunning  = true;
    dataManager->isDemuxRunning= true;
    dataManager->isVideoRunning= true;
    dataManager->isPause =false;
    dataManager->isPausing =false;
    dataManager->videoPauseReady =false;
    dataManager->audioPauseReady =false;
    dataManager->demuxPauseReady =false;
    dataManager->isExit = false;
    dataManager->currentAudioPts = LONG_LONG_MAX;

    std::thread demux_th(&LammyOpenglVideoPlayer::demuxThreadMain,this);
    demux_th.detach();

    std::thread opensles_th(&LammyOpenglVideoPlayer::OpenSLESThreadMain,this);
    opensles_th.detach();

    std::thread audio_th(&LammyOpenglVideoPlayer::audioThreadMain,this);
    audio_th.detach();

    std::thread video_th(&LammyOpenglVideoPlayer::videoThreadMain,this);
    video_th.detach();
}



void LammyOpenglVideoPlayer::onSurfaceDestroyed()
{
    LOGE("pause type is dataManager->isPause = %d", dataManager->isPause);
//            if(dataManager->isPause)
//            {
//                pauseOrContinue();
//            }
    LOGE("pause type is dataManager->isPause 2 = %d", dataManager->isPause);
            stopThread();
           // dataManager->isPause = true;
}



void LammyOpenglVideoPlayer::initView(  ANativeWindow *win)
{
    dataManager -> win = win;
    LEGL::Get()->Init(dataManager->win);
    openglVideoShow->Init();
}

bool LammyOpenglVideoPlayer::onSurfaceCreated(ANativeWindow *win)
{
    dataManager -> win = win;
    if(dataManager->avFormatContext != 0)
    {
        startThread();
        return true;
    }
    return false;
}









