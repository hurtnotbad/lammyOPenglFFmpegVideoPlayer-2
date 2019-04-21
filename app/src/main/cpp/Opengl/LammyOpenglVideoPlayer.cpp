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
    LEGL::Get()->Init(dataManager->win);
    openglVideoShow->Init();

   while(!dataManager->isVideoExit)
    {

        if(dataManager->videoPauseReady){
            LSleep(2);
            LOGI("pause videoPauseReady  .....");
            continue;
        }

        AVFrame *  avFrame = ffMdecode->decode(0);
        if(avFrame != 0 && avFrame != nullptr){
          LOGI("avFrame video show  .....");
            openglVideoShow->show(avFrame);
        }
        else if(avFrame == 0){
            LSleep(2);
        }

        if(dataManager->isPausing)//&&dataManager->demuxPauseReady
        {
            LOGI("pause videoPauseReady  .open....");
            dataManager->videoPauseReady = true;
        }

    }
        dataManager->isVideoRunning = false;
//        LEGL::Get()->Close();
   
}
void LammyOpenglVideoPlayer::startVideo()
{
    dataManager->isVideoExit = false;
    dataManager->isVideoRunning  = true;
    std::thread video_th(&LammyOpenglVideoPlayer::videoThreadMain,this);
    video_th.detach();
}


void LammyOpenglVideoPlayer::audioThreadMain()
{
    while(!dataManager->isExit)
    {
        while(dataManager->audioPauseReady)
        {
            LSleep(2);
            //continue;
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


}

void LammyOpenglVideoPlayer::demuxThreadMain()
{
    while(!dataManager->isExit)
    {

        while(dataManager->demuxPauseReady)
        {
            LSleep(2);
            continue;
        }
        /********************* 解封装部分****************************/
        int mode = ffmDemux->demux();
        /********************* 解封装部分结束****************************/

        if(dataManager->isPausing)
        {
            LOGI("pause demuxPauseReady  .open....");
            dataManager->demuxPauseReady = true;
        }

    }
    dataManager->isDemuxRunning= false;

}


void LammyOpenglVideoPlayer::play(const char * videoPath)
{

    if(dataManager -> avFormatContext != nullptr)
    {
        close();
    }

    LOGI("LammyVideoPlayer play .....0");
    dataManager->url = videoPath;
    dataManager->isVideoRunning  = true;
    dataManager->isAudioRunning  = true;
    dataManager->isDemuxRunning= true;
    dataManager->isPause =false;
    dataManager->isPausing =false;
    dataManager->videoPauseReady =false;
    dataManager->audioPauseReady =false;
    dataManager->isExit = false;
    dataManager->currentAudioPts = LONG_LONG_MAX;
    LOGI("LammyVideoPlayer play .....1");
    std::thread demux_th(&LammyOpenglVideoPlayer::demuxThreadMain,this);
    demux_th.detach();
    LOGI("LammyVideoPlayer play .....2");
    std::thread opensles_th(&LammyOpenglVideoPlayer::OpenSLESThreadMain,this);
    opensles_th.detach();
    LOGI("LammyVideoPlayer play .....3");
    std::thread audio_th(&LammyOpenglVideoPlayer::audioThreadMain,this);
    audio_th.detach();
    LOGI("LammyVideoPlayer play .....4");
//    std::thread video_th(&LammyOpenglVideoPlayer::videoThreadMain,this);
//    video_th.detach();

    LOGI("LammyVideoPlayer play .....end");

}

void LammyOpenglVideoPlayer::close()
{
    if(dataManager->avFormatContext == nullptr)
        return;
    stopThread();
    openSLESAudioPlayer->Close();
    dataManager->clearData();
    dataManager->closeFFMpeg();

}

bool LammyOpenglVideoPlayer::stopThread()
{
    dataManager->isExit = true;
    for(int i = 0 ; i < 200; i++)
    {
        if( !dataManager->isDemuxRunning && !dataManager->isAudioRunning){// !dataManager->isVideoRunning&&
            LOGE("stop thread success !");
            return true;
        }
        LSleep(1);
    }
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





