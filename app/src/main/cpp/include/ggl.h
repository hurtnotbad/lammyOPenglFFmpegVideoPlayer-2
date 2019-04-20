//
// Created by zhangpeng30 on 2019/3/19.
//


#pragma
#include <jni.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <android/asset_manager_jni.h>
#include <android/asset_manager.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <string>
#include <sstream>
#include <vector>
#include <functional>
#include <map>


//
//#define __DEBUG__ANDROID__ON
////write debug images
//#ifdef  __DEBUG__ANDROID__ON
//#include <android/log.h>
//// Define the LOGI and others for print debug infomation like the log.i in java
//#define LOG_TAG    "lammy-jni-log:"
////#undef LOG
//#define LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG, __VA_ARGS__)
//#define LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG, __VA_ARGS__)
//#define LOGW(...)  __android_log_print(ANDROID_LOG_WARN,LOG_TAG, __VA_ARGS__)
//#define LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG, __VA_ARGS__)
//
//#endif



