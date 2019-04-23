//
// Created by lammy on 2019/4/13.
//

#ifndef LAMMYOPENGLVIDEOPLAYER_LAMMYOPENGLVIDEOPLAYER_H
#define LAMMYOPENGLVIDEOPLAYER_LAMMYOPENGLVIDEOPLAYER_H


#include "DataManager.h"
#include "FFMDemux.h"
#include "FFMdecode.h"
#include "FFMVideoShow.h"
#include "FFMResample.h"
#include "OpenSLESAudioPlayer.h"
#include "OpenglShow.h"

class LammyOpenglVideoPlayer {
public:
    DataManager *dataManager;
    FFMDemux *ffmDemux ;
    FFMdecode *ffMdecode;
    OpenglShow *openglVideoShow;
    FFMResample* ffmResample;
    OpenSLESAudioPlayer* openSLESAudioPlayer;
    int test = 3;


public:
    LammyOpenglVideoPlayer();


    void play(const char * videoPath);
    void pauseOrContinue();
    void close();
    void seekTo(float progress);

    void startVideo();
    bool stopVideo();
    void demuxThreadMain();
    void videoThreadMain();
    void seekThreadMain();
    void audioThreadMain();
    void OpenSLESThreadMain();
    bool stopThread();

    void clearOpengl();
};


#endif //LAMMYOPENGLVIDEOPLAYER_LAMMYOPENGLVIDEOPLAYER_H
