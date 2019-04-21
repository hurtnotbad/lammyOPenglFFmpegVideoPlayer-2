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
    LOGE("opengl  SetViewPortSize。。。。。");
    glViewport(0,0,width,height);
}

//void OpenglShow::Draw(AVFrame *avFrame)//LammyOpenglVideoPlayer* lammyOpenglVideoPlayer
//{
////    LammyOpenglVideoPlayer* lammyOpenglVideoPlayer = ( LammyOpenglVideoPlayer*)lammyOpenglVideoPlayer1;
////    if(lammyOpenglVideoPlayer->dataManager->isVideoRunning&&!lammyOpenglVideoPlayer->dataManager->isPause){
////        LOGE("OpenglShow  Draw。 video。。。。");
////        lammyOpenglVideoPlayer->videoThreadMain();
////    }
//    show(avFrame);
//
//}

void OpenglShow::show(AVFrame *avFrame)
{

    int w = avFrame->width;
    int h = avFrame->height;

//    unsigned char * y  = avFrame->data[0];
//    unsigned char * u  = avFrame->data[0] + h*w ;
//    unsigned char * v  = avFrame->data[0]+ h*w + h*w/4;

    unsigned char * y  = avFrame->data[0];
    unsigned char * u  = avFrame->data[1] ;
    unsigned char * v  = avFrame->data[2];

    glProgram->Draw(w,h,y,u ,v);


//    LOGE("LEGL3 += %lld" , legl);
    LEGL::Get()->Draw();
    LOGE("LEGL3 += ");
    av_frame_free(&avFrame);
}


