//
// Created by lammy on 2019/4/13.
//



#include <Log.h>
#include <OpenglShow.h>
#include <LammyOpenglVideoPlayer.h>
#include <LEGL.h>

#include "OpenglShow.h"
OpenglShow::OpenglShow(void *datamager)
{
     this->datamager = datamager;
     glProgram = new GLProgram();
}

void OpenglShow::Init()
{
    LOGE("opengl  Init。。。。。");
    glProgram->init();
}

void OpenglShow::SetViewPortSize(float width, float height)
{
    windowHeight = width;
    windowHeight = height;
}

void OpenglShow::show(AVFrame *avFrame)
{

    int w = avFrame->width;
    w =w -  w%64;
    int h = avFrame->height;

//    unsigned char * y  = avFrame->data[0];
//    unsigned char * u  = avFrame->data[0] + h*w ;
//    unsigned char * v  = avFrame->data[0]+ h*w + h*w/4;

    unsigned char * y  = avFrame->data[0];
    unsigned char * u  = avFrame->data[1] ;
    unsigned char * v  = avFrame->data[2];


    glProgram->Draw(w,h,y,u ,v);
    LEGL::Get()->Draw();

    av_frame_free(&avFrame);
}

int *OpenglShow::getsShowSize(AVFrame *avFrame)
{
    int w = avFrame->width;
    int h = avFrame->height;

    int size[2];

//    float

    return size;

}


