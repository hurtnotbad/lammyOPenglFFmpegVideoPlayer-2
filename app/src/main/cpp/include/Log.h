//
// Created by lammy on 2019/4/3.
//

#ifndef LAMMYVIDEO_LOG_H
#define LAMMYVIDEO_LOG_H

#include <android/log.h>
#define ANDROID
#ifdef ANDROID
#include <android/log.h>
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG,"Lammy-jni",__VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,"Lammy-jni",__VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,"Lammy-jni",__VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN,"Lammy-jni",__VA_ARGS__)
#else
#define LOGD(...) printf("XPlay",__VA_ARGS__)
#define LOGI(...) printf("XPlay",__VA_ARGS__)
#define LOGE(...) printf("XPlay",__VA_ARGS__)

#endif

#endif //LAMMYVIDEO_LOG_H
