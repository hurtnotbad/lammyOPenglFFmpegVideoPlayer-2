//
// Created by lammy on 2019/4/20.
//

#include <Log.h>
#include <android/native_window_jni.h>
#include <ggl.h>
#include "LEGL.h"



void LEGL::Draw()
{
    mux.lock();
    if(display == EGL_NO_DISPLAY || surface == EGL_NO_SURFACE)
    {
        LOGE("draw..EGL_NO_DISPLAY.....................................................1111111111111111.");
        mux.unlock();
        return;
    }
    LOGE("draw................................................................2222222222222222222222222222");
    bool re = eglSwapBuffers(display,surface);

   if(re == EGL_FALSE)
   {
       LOGE("draw failed ......");
   }
    LOGE("draw success ......");
    mux.unlock();
}
 void LEGL::Close()
{
    mux.lock();
    if(display == EGL_NO_DISPLAY)
    {
        mux.unlock();
        return;
    }
    eglMakeCurrent(display,EGL_NO_SURFACE,EGL_NO_SURFACE,EGL_NO_CONTEXT);

    if(surface != EGL_NO_SURFACE)
        eglDestroySurface(display,surface);
    if(context != EGL_NO_CONTEXT)
        eglDestroyContext(display,context);

    eglTerminate(display);

    display = EGL_NO_DISPLAY;
    surface = EGL_NO_SURFACE;
    context = EGL_NO_CONTEXT;
    mux.unlock();
}



bool LEGL::Init(void *win)
{
    ANativeWindow *nwin = (ANativeWindow *)win;
    Close();
    //初始化EGL
    mux.lock();
    //1 获取EGLDisplay对象 显示设备
    display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    if(display == EGL_NO_DISPLAY)
    {
        mux.unlock();
        LOGE("eglGetDisplay failed!");
        return false;
    }
    LOGE("eglGetDisplay success!");
    //2 初始化Display
    if(EGL_TRUE != eglInitialize(display,0,0))
    {
        mux.unlock();
        LOGE("eglInitialize failed!");
        return false;
    }
    LOGE("eglInitialize success!");

    //3 获取配置并创建surface
    EGLint configSpec [] = {
            EGL_RED_SIZE,8,
            EGL_GREEN_SIZE,8,
            EGL_BLUE_SIZE,8,
            EGL_SURFACE_TYPE,EGL_WINDOW_BIT,
            EGL_NONE
    };
//    EGLint surfaceAttr[] = {
//            EGL_WIDTH, 1920,
//            EGL_HEIGHT, 1080,
//            EGL_NONE
//    };
    EGLConfig config = 0;
    EGLint numConfigs = 0;
    if(EGL_TRUE != eglChooseConfig(display,configSpec,&config,1,&numConfigs))
    {
        mux.unlock();
        LOGE("eglChooseConfig failed!");
        return false;
    }
    LOGE("eglChooseConfig success!");
    surface = eglCreateWindowSurface(display,config,nwin,NULL);
//    surface =  eglCreatePbufferSurface(display, config, surfaceAttr);

    //4 创建并打开EGL上下文
    const EGLint ctxAttr[] = { EGL_CONTEXT_CLIENT_VERSION ,2, EGL_NONE};
    context = eglCreateContext(display,config,EGL_NO_CONTEXT,ctxAttr);
    if(context == EGL_NO_CONTEXT)
    {
        mux.unlock();
        LOGE("eglCreateContext failed!");
        return false;
    }
    LOGE("eglCreateContext success!");

    if(EGL_TRUE != eglMakeCurrent(display,surface,surface,context))
    {
        mux.unlock();
        LOGE("eglMakeCurrent failed!");
        return false;
    }
    LOGE("eglMakeCurrent success!");
    mux.unlock();
    return true;


}



LEGL *LEGL::Get()
{
    static LEGL egl;
    return &egl;
}

