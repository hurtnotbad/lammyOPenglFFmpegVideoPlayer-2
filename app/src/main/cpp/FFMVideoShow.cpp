//
// Created by lammy on 2019/4/8.
//

#include <Log.h>
#include "FFMVideoShow.h"

FFMVideoShow::FFMVideoShow(DataManager *dataManager)
{
    this->dataManager = dataManager;
}


void  FFMVideoShow::show(AVFrame *avFrame)
{

    if(dataManager->swsContext == nullptr){
//        dataManager -> outWidth = avFrame->width;
//        dataManager -> outHeight = avFrame->height;

//        dataManager -> outWidth = dataManager -> outWidth / 64 * 64;
//        dataManager -> outHeight = avFrame->height >= 1080 ?1080-4:avFrame->height;

        LOGE("outWidth = %d , outheight = %d", dataManager -> outWidth, dataManager -> outHeight );
//        dataManager -> outWidth= 1280;
//        dataManager -> outHeight = 720;
        dataManager->rgb = new char[dataManager -> outWidth * dataManager -> outHeight * 4];
        ANativeWindow_setBuffersGeometry(dataManager->aNativeWindow, dataManager->outWidth, dataManager->outHeight, WINDOW_FORMAT_RGBA_8888);
//        ANativeWindow_setBuffersGeometry(dataManager->aNativeWindow, 2012, 1080, WINDOW_FORMAT_RGBA_8888);

        // 这个开销也比较大，且经常失败，，这里不为空的时候，再去获取，发现视频解码速度快了很多。
        dataManager->swsContext = sws_getCachedContext(dataManager->swsContext,
                                                       avFrame->width,
                                                       avFrame->height,
                                                       (AVPixelFormat)avFrame->format,
                                                       dataManager->outWidth,
                                                       dataManager->outHeight,
                                                       AV_PIX_FMT_RGBA,
                                                       SWS_FAST_BILINEAR,
                                                       0,0,0 );

    }


    if(!dataManager->swsContext){
        LOGE("swsContext failed  %d ", dataManager->swsContext);
    }
    else {
        LOGE("show show 3" );
//        LOGE("swsContext success  %d!", dataManager->swsContext);
        // 指针数组AV_NUM_DATA_POINTERS=8 不一定全用
        uint8_t *data[AV_NUM_DATA_POINTERS]={0};
        if(dataManager->rgb != 0)
        data[0]= (uint8_t *)dataManager->rgb;

        int lines[AV_NUM_DATA_POINTERS]={0};
        lines[0]=dataManager->outWidth*4;
        int h = sws_scale(dataManager->swsContext,
                          (const uint8_t **)avFrame->data,
                          avFrame->linesize,0,
                          avFrame->height,
                          data,
                          lines
        );
        LOGE("show show 4   h =%d" , h );
        av_frame_free(&avFrame);
        if(h >0)
        {
            LOGE("show show 5" );
            ANativeWindow_lock(dataManager->aNativeWindow, &(dataManager->wbuf), 0);
            uint8_t  *dst = (uint8_t*)dataManager->wbuf.bits;
            memcpy(dst, dataManager->rgb,dataManager->outHeight*dataManager->outWidth*4);
            ANativeWindow_unlockAndPost(dataManager->aNativeWindow);
        }
    }


}

