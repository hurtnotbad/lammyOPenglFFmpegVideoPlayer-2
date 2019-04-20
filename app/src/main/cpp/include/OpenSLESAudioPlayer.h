//
// Created by lammy on 2019/4/8.
//

#ifndef LAMMYVIDEOPALYER_OPENSLESAUDIOPLAYER_H
#define LAMMYVIDEOPALYER_OPENSLESAUDIOPLAYER_H


#include <DataManager.h>
#include <SLES/OpenSLES.h>

class OpenSLESAudioPlayer {

public:
    OpenSLESAudioPlayer(DataManager * dataManager);

    bool playAudio();
    void  getAudioData();
    void PlayCall(void *bufq);
    void Close();
public:
    DataManager * dataManager;
    unsigned char *buf = 0;

};


#endif //LAMMYVIDEOPALYER_OPENSLESAUDIOPLAYER_H
