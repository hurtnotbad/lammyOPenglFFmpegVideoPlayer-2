//
// Created by lammy on 2019/4/8.
//

#ifndef LAMMYVIDEOPALYER_FFMVIDEOSHOW_H
#define LAMMYVIDEOPALYER_FFMVIDEOSHOW_H


#include <DataManager.h>

class FFMVideoShow {
public:
    FFMVideoShow(DataManager * dataManager);
    void show(AVFrame *avFrame);

public:
    DataManager *dataManager;

};


#endif //LAMMYVIDEOPALYER_FFMVIDEOSHOW_H
