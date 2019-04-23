//
// Created by lammy on 2019/4/7.
//

#include <DataManager.h>
#include <Log.h>

#include "DataManager.h"
void LSleep(int mis)
{
    std::chrono::milliseconds du(mis);
    std::this_thread::sleep_for(du);
}

long long GetNowMs(){
    struct timeval tv;
    gettimeofday(&tv, NULL);//  tv.tv_sec 很大，%36000 取得100个小时前的时间，不除也可以
    int sec = tv.tv_sec%36000;
    long long t = sec*1000 + tv.tv_usec/1000;
    return t;
}
float getFloatValue(AVRational a )
{
    LOGE("getFloatValue");
    return  a.den==0||a.num==0?0.0:(float(a.num)/(float(a.den)));
}


void DataManager::clearData()
{
    LOGE("clearData  start");
    // 清理缓存
    avformat_flush(avFormatContext);
    LOGE("clearData video start");
    videoLock.lock();
    LOGE("clearData video start 1");
    while(videoPackets.size()>0)
    {
        AVPacket *avPacket =  videoPackets.front();
        videoPackets.pop_front();
        av_packet_unref(avPacket);
    }
    LOGE("clearData video start 2");
    if(videoCodecContext != 0){
        avcodec_flush_buffers(videoCodecContext);
    }

    LOGE("clearData video start 3");
    videoLock.unlock();

    LOGE("clearData audio start");

    audioLock.lock();

    while(audioPackets.size() >0)
    {
        AVPacket* data =  audioPackets.front();
        av_packet_unref(data);
        audioPackets.pop_front();

    }

    while(audioData.size()>0)
    {
        uint8_t * data =  audioData.front();
        audioData.pop_front();
        if(data!=0)
        free(data);
    }
    if(audioCodecContext != 0){
        avcodec_flush_buffers(audioCodecContext);
    }

    while(audioPts.size() >0)
    {
        audioPts.pop_front();
    }
    currentAudioPts =LONG_LONG_MAX;
    audioLock.unlock();
    LOGE("clearData  end");
}

void DataManager::closeFFMpeg()
{
    if(videoCodecContext != 0){
        avcodec_close(videoCodecContext);
    }
    if(audioCodecContext != 0){
        avcodec_close(audioCodecContext);
    }
    if(0 != swrContext)
    {
        swr_close(swrContext);
    }
    if(0 != swsContext)
    {
        sws_freeContext(swsContext);
    }
    if(avFormatContext != 0 )
    {
        avformat_close_input(&avFormatContext);
    }
//    avcodec_close(videoCodecContext);
//    avcodec_close(audioCodecContext);
//    swr_close(swrContext);
//    sws_freeContext(swsContext);
//    avformat_close_input(&avFormatContext);
    swrContext = nullptr;
    swsContext = nullptr;
    videoCodecContext = nullptr;
    audioCodecContext = nullptr;
    avFormatContext = nullptr;
    videoCodec = nullptr;
    audioCodec = nullptr;

}
