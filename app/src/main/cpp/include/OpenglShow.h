//
// Created by lammy on 2019/4/13.
//

#ifndef LAMMYOPENGLVIDEOPLAYER_OPENGLSHOW_H
#define LAMMYOPENGLVIDEOPLAYER_OPENGLSHOW_H

extern "C"{
#include <libavutil/frame.h>
};

#include "GLProgram.h"
//#include "LammyOpenglVideoPlayer.h"

class OpenglShow {
public:
    OpenglShow(void *datamager);
    void Init();
    void SetViewPortSize(float width , float height);
//    void Draw(AVFrame *avFrame);//LammyOpenglVideoPlayer* lammyOpenglVideoPlayer

    void show(AVFrame *avFrame);
public:
    void *datamager;
    GLProgram * glProgram;
};


#endif //LAMMYOPENGLVIDEOPLAYER_OPENGLSHOW_H
