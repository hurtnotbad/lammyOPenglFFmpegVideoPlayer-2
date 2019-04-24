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


public:
    LammyOpenglVideoPlayer();

    void play(const char * videoPath);
    void start(const char * videoPath);
    void pauseOrContinue();
    void close();
    void seekTo(float progress);
    void initView(  ANativeWindow *win);



    void demuxThreadMain();
    void videoThreadMain();
    void seekThreadMain();
    void audioThreadMain();
    void OpenSLESThreadMain();

    bool stopThread();
    void startThread();


    void onSurfaceDestroyed();
    bool onSurfaceCreated(ANativeWindow *win);

    void clearOpengl();
};


#endif //LAMMYOPENGLVIDEOPLAYER_LAMMYOPENGLVIDEOPLAYER_H
