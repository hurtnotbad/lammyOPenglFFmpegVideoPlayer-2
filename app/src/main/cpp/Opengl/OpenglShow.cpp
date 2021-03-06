//
// Created by lammy on 2019/4/13.
//

#include <Log.h>
#include <OpenglShow.h>
#include <LammyOpenglVideoPlayer.h>
#include <LEGL.h>
#include <cstring>


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
    windowWidth = width;
    windowHeight = height;
    LOGE("windowWidth = %d, windowHeight = %d", windowWidth, windowHeight );
}
unsigned char * y = nullptr;
unsigned char * u = nullptr;
unsigned char * v = nullptr;
void getYUV420pData(AVFrame *avFrame)
{
    int w = avFrame->width;
    int h = avFrame->height;

    if(y == nullptr){
        y = new unsigned char[w*h];
    }
    if(u == nullptr){
        u = new unsigned char[w*h/4];
    }
    if(v == nullptr){
        v = new unsigned char[w*h/4];
    }


    int l1 = avFrame->linesize[0];
    int l2 = avFrame->linesize[1];
    int l3 = avFrame->linesize[2];
    for(int i= 0 ; i < h ; i++)
    {
        memcpy(y + w*i,avFrame->data[0] + l1* i, sizeof( unsigned char)*w);
    }
    for(int i= 0 ; i < h/2 ; i++)
    {
        memcpy(u + w/2*i,avFrame->data[1] +  l2 * i, sizeof( unsigned char)*w/2);
        memcpy(v + w/2*i,avFrame->data[2] + l3 * i, sizeof( unsigned char)*w/2);
    }
}

void getNV12Data(AVFrame *avFrame)
{
    int w = avFrame->width;
    int h = avFrame->height;

    if(y == nullptr){
        y = new unsigned char[w*h];
    }
    if(u == nullptr){
        u = new unsigned char[w*h/2];
    }
    LOGE("avFrame->linesize[0] = %d" ,avFrame->linesize[0]);
    LOGE("avFrame->linesize[1] = %d" ,avFrame->linesize[1]);

    int l1 = avFrame->linesize[0];
    int l2 = avFrame->linesize[1];
    for(int i= 0 ; i < h ; i++)
    {
        memcpy(y + w*i,avFrame->data[0] + l1* i, sizeof( unsigned char)*w);
    }
    for(int i= 0 ; i < h/2 ; i++)
    {
        memcpy(u + w*i,avFrame->data[1] +  l2 * i, sizeof( unsigned char)*w);
    }
}



void OpenglShow::show(AVFrame *avFrame)
{

//    // 直接按照填补的 宽度来显示，效率最高，若视频宽度不是64/32（根据系统位数）倍数，右边会存在边框。
//    int w = avFrame->linesize[0];
//    int h = avFrame->height;
//    unsigned char * y  = avFrame->data[0];
//    unsigned char * u  = avFrame->data[1] ;
//    unsigned char * v  = avFrame->data[2];
//    glProgram->Draw(w,h,y,u ,v);

//    AVPixelFormat
//LOGE("AVPixelFormat = %d" ,avFrame->format); //------------0
//LOGE("AV_PIX_FMT_NV12 = %d" ,AV_PIX_FMT_NV12);//------------25
//LOGE("AV_PIX_FMT_NV21 = %d" ,AV_PIX_FMT_NV21);//------------26

    int w = avFrame->width;
    int h = avFrame->height;
    getsShowSize(w,h);
    switch(avFrame->format)
    {
        case AV_PIX_FMT_YUV420P:
            getYUV420pData(avFrame);
            glProgram->Draw(w,h,y,u ,v ,AV_PIX_FMT_YUV420P ,showRect);
            break;
        case AV_PIX_FMT_NV12:
            getNV12Data(avFrame);
            glProgram->Draw(w,h,y,u , nullptr ,AV_PIX_FMT_NV12 ,showRect);
            break;
        case AV_PIX_FMT_NV21:
            getNV12Data(avFrame);// 与getNV12Data 一样的，只是在着色器里面取 U V的顺序不同
            glProgram->Draw(w,h,y,u , nullptr ,AV_PIX_FMT_NV21,showRect );
            break;

    }

    LEGL::Get()->Draw();
    av_frame_free(&avFrame);
}


void OpenglShow::getsShowSize(int w ,int h)
{

    int newH = 0;
    int newW = 0;

    float rWin = float( w)/ float(h);
    float rFra = float(windowHeight)/ float(windowWidth);
    if(rWin > rFra )
    {
        newH = windowHeight;
        float r =  float(newH)/ float(h);
        newW = int(r * w);
    } else{

        newW = windowWidth;
        float r =  float(newW)/ float(w);
        newH = int(r * h);

    }

    int left =(windowWidth - newW) /2;
    int top =(windowHeight - newH) /2;
    showRect[0] = left;
    showRect[1] = top;
    showRect[2] = newW;
    showRect[3] = newH;

}

void OpenglShow::freeSpace()
{
    free(y);
    free(u);
    free(v);
    y = nullptr;
    u = nullptr;
    v = nullptr;
}


