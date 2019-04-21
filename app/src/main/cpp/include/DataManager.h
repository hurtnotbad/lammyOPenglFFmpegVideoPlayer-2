//
// Created by lammy on 2019/4/7.
//

#ifndef LAMMYVIDEOPALYER_DATAMANAGER_H
#define LAMMYVIDEOPALYER_DATAMANAGER_H

#include "android/native_window_jni.h"
#include "android/native_window.h"
#include "LEGL.h"

#include <list>
#include <thread>

extern "C"{
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include "libswscale/swscale.h"
#include "libswresample/swresample.h"
};

void LSleep(int mis);
float getFloatValue(AVRational a );
long long GetNowMs();


class DataManager {
public:
    DataManager(){};
    void clearData();
    void closeFFMpeg();


    ANativeWindow *win;

    /********************* 播放器状态******************************/
    bool isExit= false;
    bool isVideoExit = false;

    bool isVideoRunning = false;
    bool isAudioRunning = false;
    bool isDemuxRunning = false;


    bool isPause = false;

    bool isPausing = false;
    bool videoPauseReady = false;
    bool audioPauseReady = false;
    bool demuxPauseReady = false;
    bool isSeeking = false;
    std::mutex pauseLock;
    std::mutex seekLock;

    float vBasetime= 0.0;
    float aBasetime= 0.0 ;



    const char * url = nullptr;

    /********************* 窗体相关数据******************************/
    ANativeWindow *aNativeWindow = nullptr;
    ANativeWindow_Buffer wbuf;
    int outWidth = 1280;
    int outHeight = 720;

    /********************* 解封装相关******************************/
    bool demuxOver = false;
    int videoStreamIndex =0;
    int audioStreamIndex =0;
    AVFormatContext * avFormatContext = nullptr;
    int64_t duration = 0;
    AVPacket *aPacket= nullptr;
    AVFrame *vFrame= nullptr;


    /********************* 解码相关******************************/
    AVCodec *videoCodec= nullptr;
    AVCodec *audioCodec= nullptr;
    AVCodecContext*audioCodecContext= nullptr;
    AVCodecContext*videoCodecContext = nullptr;
//    AVFrame * avFrame= nullptr;


    /********************* FFMpeg 显示相关******************************/
    SwsContext *swsContext = nullptr;
    char *rgb  = nullptr;

    /********************* 音频resample相关******************************/
    SwrContext *swrContext = nullptr;
    int audioOutChannel =2 ;
    int audioDateSize = 0;
    int sample_rate =0;
    AVSampleFormat  AV_SAMPLE_FMT_S16;

    int maxSizeAudio = 250;
    std::list <uint8_t * > audioData ;
    std::mutex audioLock;

    /********************* 音视频同步相关******************************/
    long long currentAudioPts = LONG_LONG_MAX;
    std::list <long long> audioPts ;

    std::mutex slesLock;
    std::list < AVPacket * > audioPackets ;

    /************************视频缓冲******************************/
    std::list < AVPacket * > videoPackets ;
    int maxVideoPackets = 200;
    std::mutex videoLock;


//    LEGL* legl = nullptr;

};


#endif //LAMMYVIDEOPALYER_DATAMANAGER_H
