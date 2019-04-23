//
// Created by lammy on 2019/4/7.
//
#include "FFMDemux.h"


FFMDemux::FFMDemux(DataManager *dataManager)
{
    this->dataManager = dataManager;

    //初始化解封装
    av_register_all();
    //初始化网络
    avformat_network_init();
    avcodec_register_all();
}

int  FFMDemux::demux()
{
    init();

    AVPacket *avPacket= av_packet_alloc();
    int re = av_read_frame(dataManager->avFormatContext,avPacket);

    if(re !=0)
    {
        av_packet_free(&avPacket);
       // dataManager->demuxOver = true;
        return -1;
    }
    else
    {
        if(avPacket->stream_index == dataManager->audioStreamIndex)
        {
            while (!dataManager->isExit)
            {
                dataManager->audioLock.lock();
                if(dataManager->audioPackets.size() <dataManager->maxVideoPackets){
                    dataManager->audioPackets.push_back(avPacket);
                    dataManager->audioLock.unlock();
                    break;
                }
                if(dataManager->isPausing){
                    dataManager->audioLock.unlock();
                    return 1;
                }
                LOGE("demux 音频 缓存 大于100了,等会儿 %d  ",dataManager->audioPackets.size());
                dataManager->audioLock.unlock();
                LSleep(1);
            }
            return 1;
        }
        else if(avPacket->stream_index == dataManager->videoStreamIndex)
        {

            while (!dataManager->isExit)
                {
                    dataManager->videoLock.lock();
                    if(dataManager->videoPackets.size() <dataManager->maxVideoPackets){
                        dataManager->videoPackets.push_back(avPacket);
                        dataManager->videoLock.unlock();
                        break;
                        }
                    if(dataManager->isPausing){
                        dataManager->videoLock.unlock();
                        return 0;
                    }
//                        LOGE("视频 缓存 大于100了,等会儿 %d  ",dataManager->videoPackets.size());
                        dataManager->videoLock.unlock();
                        LSleep(1);
                }
            return 0;
        }
        else
         {
            av_packet_free(&avPacket);
            return -1;
         }

    }

}


void FFMDemux::init()
{
    LOGE("FFMDemux::init");
    if(  dataManager->avFormatContext == 0)
    {
        AVFormatContext *ic = NULL;
        // 会将url存入AVFormatContext，释放掉 dataManager->url
        int re = avformat_open_input(&ic,dataManager->url,0,0);
        if(re != 0)
        {
            LOGE("avformat_open_input failed!:%s",av_err2str(re));
        }
        avformat_find_stream_info(ic, 0);
        dataManager->duration =ic->duration;
        LOGE("duration = %lld nb_streang = %d",ic->duration, ic->nb_streams);
        //start();

        int videoStream = av_find_best_stream(ic, AVMEDIA_TYPE_VIDEO,-1,-1,NULL, 0);
        int audioStream = av_find_best_stream(ic, AVMEDIA_TYPE_AUDIO,-1,-1,NULL, 0);
        dataManager->videoStreamIndex =videoStream;
        dataManager->audioStreamIndex =audioStream;
        dataManager->avFormatContext =ic;
        LOGE("FFMDemux::init start ..........success");
    }


}

bool FFMDemux::seekTo(float pos)
{
    if(pos < 0 || pos >1)
    {
        return false;
    }

    bool re;

    if(dataManager->avFormatContext == 0)
    {
        return false;
    }

    long long pos2 = dataManager->avFormatContext->streams[dataManager->videoStreamIndex]->duration* pos;
    re = av_seek_frame(dataManager->avFormatContext, dataManager->videoStreamIndex, pos2, AVSEEK_FLAG_FRAME|AVSEEK_FLAG_BACKWARD);
    return re;

}
