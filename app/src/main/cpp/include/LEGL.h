//
// Created by lammy on 2019/4/20.
//

#ifndef LAMMYOPENGLFFMPEGVIDEOPLAYER_2_LEGL_H
#define LAMMYOPENGLFFMPEGVIDEOPLAYER_2_LEGL_H


#include <mutex>
#include <EGL/egl.h>
#include "DataManager.h"

class LEGL {
public:
    EGLDisplay display = EGL_NO_DISPLAY;
    EGLSurface surface = EGL_NO_SURFACE;
    EGLContext context = EGL_NO_CONTEXT;
    std::mutex mux;

public:
     bool Init(void *win);
     void Close();
     void Draw();
     static LEGL *Get();
protected:
    LEGL(){}
};


#endif //LAMMYOPENGLFFMPEGVIDEOPLAYER_2_LEGL_H
