//
// Created by lammy on 2019/4/13.
//

#ifndef LAMMYOPENGLVIDEOPLAYER_OPENGLSHOW_H
#define LAMMYOPENGLVIDEOPLAYER_OPENGLSHOW_H


#include "DataManager.h"
#include "GLProgram.h"
//#include "LammyOpenglVideoPlayer.h"

class OpenglShow {
public:
    OpenglShow(DataManager *datamager);
    void Init();
    void SetViewPortSize(float width , float height);
    void Draw(void * lammyOpenglVideoPlayer);//LammyOpenglVideoPlayer* lammyOpenglVideoPlayer

    void show(AVFrame *avFrame);
public:
    DataManager *datamager;
    GLProgram * glProgram;
};


#endif //LAMMYOPENGLVIDEOPLAYER_OPENGLSHOW_H
