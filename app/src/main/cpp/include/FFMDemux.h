//
// Created by lammy on 2019/4/7.
//

#ifndef LAMMYVIDEOPALYER_FFMDEMUX_H
#define LAMMYVIDEOPALYER_FFMDEMUX_H

#include "FFMDemux.h"
#include "FFMpeg.h"


class FFMDemux {
public:
    FFMDemux(DataManager* dataManager);

    void init();
     int demux();
    bool seekTo(float pos);

public:
    DataManager* dataManager;
};


#endif //LAMMYVIDEOPALYER_FFMDEMUX_H
