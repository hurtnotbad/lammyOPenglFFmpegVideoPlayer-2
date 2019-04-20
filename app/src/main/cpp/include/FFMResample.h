//
// Created by lammy on 2019/4/8.
//

#ifndef LAMMYVIDEOPALYER_FFMRESAMPLE_H
#define LAMMYVIDEOPALYER_FFMRESAMPLE_H


#include <DataManager.h>

class FFMResample {
public:
    FFMResample(DataManager* dataManager);
    void init();
    void resample(AVFrame *avFrame);
    uint8_t * getOutAudioData(AVFrame *avFrame);
public:
    DataManager *dataManager;

};


#endif //LAMMYVIDEOPALYER_FFMRESAMPLE_H
