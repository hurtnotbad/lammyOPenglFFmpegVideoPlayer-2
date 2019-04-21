//
// Created by lammy on 2019/4/8.
//

#include <Log.h>
#include "FFMResample.h"

FFMResample::FFMResample(DataManager *dataManager)
{
    this->dataManager = dataManager;

}

void FFMResample::init()
{
    //sample_fmt 采样格式
    dataManager->swrContext = swr_alloc();
    dataManager->swrContext=swr_alloc_set_opts(
            dataManager->swrContext,
            av_get_default_channel_layout(2),
            AV_SAMPLE_FMT_S16,dataManager->audioCodecContext->sample_rate,
            av_get_default_channel_layout(dataManager->audioCodecContext->channels),
            dataManager->audioCodecContext->sample_fmt, dataManager->audioCodecContext->sample_rate,
            0,0
    );
   int re = swr_init( dataManager->swrContext);
    if(re != 0) {
        LOGW("swr_init failed");
        return  ;
    }else{
        LOGW("swr_init success");
    }


}

// new 出来的空间，需要调用者释放
uint8_t * FFMResample::getOutAudioData(AVFrame *avFrame){
    //输出空间的分配
    uint8_t *out = nullptr;
    if(dataManager-> audioDateSize <= 0)
    {
        dataManager-> audioDateSize = dataManager->audioOutChannel * avFrame->nb_samples * av_get_bytes_per_sample(AV_SAMPLE_FMT_S16);
        dataManager->sample_rate = avFrame->sample_rate;
    }
    out = new unsigned char[dataManager-> audioDateSize];
    return out;
}

void FFMResample::resample(AVFrame *avFrame)
{

//    LOGE("audio resample ");
    if(dataManager->swrContext == nullptr)
    {
        init();
    }

    uint8_t *out[2] = {0};
    out[0] = getOutAudioData(avFrame);

    //nb_samples 单声道样本数量，一般1024个
    int len = swr_convert(
            dataManager->swrContext,out,
            avFrame->nb_samples,
            (const uint8_t **)avFrame->data,
            avFrame->nb_samples);
    if(len > 0){

        av_frame_free(&avFrame);
        while(!dataManager->isExit)
        {
            dataManager->audioLock.lock();
//            LOGE("audio resample 成功了");
            if(dataManager->audioData.size() <dataManager->maxSizeAudio)
            {
                (dataManager->audioData).push_back(out[0]);
                 dataManager->audioLock.unlock();
                 return;
            }else{
//                LOGE("resample 音频 缓存 大于%d了,等会儿",dataManager->audioData.size());
                dataManager->audioLock.unlock();
                LSleep(5);
                continue;
            }

        }



    } else{
        LOGE("audio resample 失败了");
    }

}
