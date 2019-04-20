//
// Created by lammy on 2019/4/7.
//

#ifndef LAMMYVIDEOPALYER_FFMDECODE_H
#define LAMMYVIDEOPALYER_FFMDECODE_H


#include <DataManager.h>

class FFMdecode {
public:
    FFMdecode(DataManager * dataManager);
    void init(int mode);

    // 0 是视频，1 是音频
    AVFrame*  decode(int mode);

public:
    DataManager * dataManager;
};


#endif //LAMMYVIDEOPALYER_FFMDECODE_H
