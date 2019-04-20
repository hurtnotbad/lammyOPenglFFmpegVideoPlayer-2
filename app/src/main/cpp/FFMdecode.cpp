//
// Created by lammy on 2019/4/7.
//

#include <Log.h>
#include <DataManager.h>

#include "FFMdecode.h"

FFMdecode::FFMdecode(DataManager *dataManager)
{
    this->dataManager = dataManager;

}

void FFMdecode::init(int mode)
{
    while(dataManager->avFormatContext == nullptr)
    {
        LOGE("dataManager->avFormatContext == nullptr    LSleep(5)");
        LSleep(5);
        continue;
    }
    if(mode == 0)
    {
        if(dataManager->videoCodecContext!= nullptr){
            return;
        }
        LOGE("videoCodec FFMdecode::init 1");
        AVCodec *videoCodec = avcodec_find_decoder(dataManager->avFormatContext->streams[dataManager->videoStreamIndex]->codecpar->codec_id);
//        AVCodec *videoCodec = avcodec_find_decoder_by_name("h264_mediacodec");
        AVCodecContext*videoCodecContext = avcodec_alloc_context3(videoCodec);
        avcodec_parameters_to_context(videoCodecContext, dataManager->avFormatContext->streams[dataManager->videoStreamIndex]->codecpar);
        videoCodecContext->thread_count = 8;
        LOGE("videoCodec FFMdecode::init 2");
        int re = avcodec_open2(videoCodecContext,0,0);
        if(re != 0) {
            LOGE("videoCodec open failed");
            return  ;
        }else{
            dataManager->videoCodec = videoCodec;
            dataManager->videoCodecContext = videoCodecContext;
            LOGI("videoCodec open  success");
        }
    }
    else
    {
        if(dataManager->audioCodecContext!= nullptr){
            return;
        }
        AVCodec *audioCodec = avcodec_find_decoder(dataManager->avFormatContext->streams[dataManager->audioStreamIndex]->codecpar->codec_id);
        AVCodecContext*audioCodecContext = avcodec_alloc_context3(audioCodec);
        avcodec_parameters_to_context(audioCodecContext, dataManager->avFormatContext->streams[dataManager->audioStreamIndex]->codecpar);
        audioCodecContext->thread_count = 8;
        int re = avcodec_open2(audioCodecContext,0,0);
        if(re != 0) {
            LOGE("audioCodec open failed");
            return  ;
        }else{
            dataManager->audioCodec = audioCodec;
            dataManager->audioCodecContext = audioCodecContext;
            LOGI("audioCodec open success");
        }

    }
}

AVFrame* FFMdecode::decode(int mode)
{

    AVPacket* avPacket = nullptr;
    if(mode == 0){
        /*************************** 视频解码****************************************/
        if(dataManager->videoCodecContext == 0) {init(0);}

        if(dataManager->vBasetime == 0.0){
            dataManager->vBasetime = 1000.0*getFloatValue((dataManager->avFormatContext->streams[dataManager->videoStreamIndex])->time_base);
        }

        dataManager->videoLock.lock();
        if(dataManager->videoPackets.size() > 0)
        {
            avPacket = dataManager->videoPackets.front();
            dataManager->videoPackets.pop_front();
        }
        dataManager->videoLock.unlock();


        if( avPacket==0 || avPacket== nullptr){
            return nullptr;
        }
        AVCodecContext*  context = dataManager->videoCodecContext;

        int re;


        re = avcodec_send_packet(context,avPacket );
        if(re!=0){
            av_packet_free(&avPacket);
            LOGE("send packet failed");
            return nullptr;
        }
        AVFrame* avFrame = av_frame_alloc();// dataManager->vFrame;
        re = avcodec_receive_frame(context,  avFrame);
//        dataManager->seekLock.unlock();

        av_packet_free(&avPacket);

        if (re == 0)
        {
            LOGE("receive frame video success ......");
            int vPts =  avFrame->pts *dataManager->vBasetime;
              while(!dataManager->isExit)
              {
                  // 如果不添加 isPause 暂停会阻塞主线程，使用上层 glsurfaceview,会导致程序崩溃
                  if(dataManager->currentAudioPts < vPts&& !dataManager->isPause )
                  {
                      LOGE("vPts = %d" , vPts);
                      LOGE("dataManager->currentAudioPts = %lld" , dataManager->currentAudioPts);
                      LSleep(1);
                      continue;
                  }
                  return avFrame;
              }

        }
            LOGE("receive video frame failed");
            return nullptr;


    }/*************************** 音频解码****************************************/
    else if(mode == 1)
    {
        if(dataManager->audioCodecContext == 0) {init(1);}


        dataManager->audioLock.lock();
        if(dataManager->audioPackets.size() > 0)
        {
            avPacket = dataManager->audioPackets.front();
            dataManager->audioPackets.pop_front();
        }
        dataManager->audioLock.unlock();

        if( avPacket==0 || avPacket== nullptr){
            return nullptr;
        }
        AVCodecContext*  context =  dataManager->audioCodecContext;

        if(dataManager->aBasetime == 0.0){
            dataManager->aBasetime = 1000.0 *getFloatValue((dataManager->avFormatContext->streams[dataManager->audioStreamIndex])->time_base);
        }

        int re;
        re = avcodec_send_packet(context,avPacket );
        if(re!=0){
            av_packet_free(&avPacket);
            LOGE("send packet failed");
            return nullptr;
        }
        av_packet_free(&avPacket);
        AVFrame* avFrame = av_frame_alloc();// dataManager->vFrame;
        re = avcodec_receive_frame(context,  avFrame);

        if (re == 0)
        {
            dataManager->audioLock.lock();
            long long pts =  avFrame->pts *dataManager->aBasetime;
            /*******************若是流媒体这里avFrame->pts 始终为 0*****************************/
//            LOGE("receive video frame sucess pts = %lld", avFrame->pts);
            dataManager->audioPts.push_back(pts);
            dataManager->audioLock.unlock();
            return avFrame;
        }else{
            LOGE("receive video frame failed");
            av_frame_free(&avFrame);
            return nullptr;
        }


    }/*************************** 都不是****************************************/
    else
        {
            LOGE("decode mode is error ......");
            return nullptr;
        }



}


