//
// Created by lammy on 2019/4/13.
//
#include <jni.h>
#include <LammyOpenglVideoPlayer.h>
#include "Log.h"

extern "C"{
#include "libavcodec/avcodec.h"
#include <libavformat/avformat.h>
#include "libavcodec/jni.h"
}

extern "C"
JNIEXPORT
jint JNI_OnLoad(JavaVM *vm, void *res) {
    av_jni_set_java_vm(vm, 0);
    // 返回jni版本
    return JNI_VERSION_1_4;
}

extern "C"
JNIEXPORT jlong JNICALL
Java_com_example_lammyopenglffmpegvideoplayer2_ILammyOpenglVideoPlayer_initNativeVideoPlayer(JNIEnv *env,
                                                                               jobject instance)
{
    LammyOpenglVideoPlayer *lammyplayer = new LammyOpenglVideoPlayer();
    return (jlong)(lammyplayer);
    // TODO

}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_lammyopenglffmpegvideoplayer2_ILammyOpenglVideoPlayer_start(JNIEnv *env,
                                                              jobject instance,
                                                              jstring videoPath_,
                                                              jobject surface,
                                                              jlong nativeVideoPlayer)
{
    const char *videoPath = env->GetStringUTFChars(videoPath_, 0);
    LammyOpenglVideoPlayer * lammyplayer =(LammyOpenglVideoPlayer *) nativeVideoPlayer;
//    ANativeWindow *win = ANativeWindow_fromSurface(env,surface);
//    lammyplayer->dataManager->win = win;
    lammyplayer->start(videoPath);

}
extern "C"
JNIEXPORT void JNICALL
Java_com_example_lammyopenglffmpegvideoplayer2_ILammyOpenglVideoPlayer_pauseOrContinue(JNIEnv *env,
                                                               jobject instance,
                                                               jlong nativeVideoPlayer)
{

    LammyOpenglVideoPlayer * lammyplayer =(LammyOpenglVideoPlayer *) nativeVideoPlayer;
    lammyplayer->pauseOrContinue();

}extern "C"
JNIEXPORT void JNICALL
Java_com_example_lammyopenglffmpegvideoplayer2_ILammyOpenglVideoPlayer_seekTo(JNIEnv *env,
                                                                jobject instance,
                                                                jfloat progress,
                                                                jlong nativeVideoPlayer)
{
    LammyOpenglVideoPlayer * lammyplayer =(LammyOpenglVideoPlayer *) nativeVideoPlayer;
    lammyplayer->seekTo(progress);

}
extern "C"
JNIEXPORT void JNICALL
Java_com_example_lammyopenglffmpegvideoplayer2_ILammyOpenglVideoPlayer_close(JNIEnv *env,
                                                               jobject instance,
                                                               jlong nativeVideoPlayer)
{

    LammyOpenglVideoPlayer * lammyplayer =(LammyOpenglVideoPlayer *) nativeVideoPlayer;
    lammyplayer->close();

}

#include "android/asset_manager.h"
#include "android/asset_manager_jni.h"
#include "utils.h"
AAssetManager* aAssetManager;

char * LoadFileContent(const char *path ){
    int filesSize = 0;
    char * fileContent = nullptr;
    AAsset * asset = AAssetManager_open(aAssetManager, path , AASSET_MODE_UNKNOWN);
    if(asset== nullptr){
        LOGE("LoadFileContent asset is null, load shader error ");
        return  nullptr;
    }
    filesSize = AAsset_getLength(asset);
    fileContent = new  char[filesSize];
    AAsset_read(asset , fileContent,filesSize);
    fileContent[filesSize]='\0';
    AAsset_close(asset);
    LOGE("LoadFileContent success ...%s",path);
    return fileContent;

}

unsigned char * LoadFileContent(const char *path , int &filesSize){
    unsigned char * fileContent = nullptr;
    AAsset * asset = AAssetManager_open(aAssetManager, path , AASSET_MODE_UNKNOWN);
    if(asset== nullptr){
        LOGE("LoadFileContent asset is null, load shader error ");
        return  nullptr;
    }

    filesSize = AAsset_getLength(asset);
    fileContent = new unsigned char[filesSize];
    AAsset_read(asset , fileContent,filesSize);
    fileContent[filesSize]='\0';
    AAsset_close(asset);
    LOGE("LoadFileContent success ...%s",path);
    return fileContent;

}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_lammyopenglffmpegvideoplayer2_ILammyOpenglVideoPlayer_InitAssetManager(JNIEnv *env,
                                                                                jclass type,
                                                                                jobject am)
{

    aAssetManager = AAssetManager_fromJava(env , am);

}


extern "C"
JNIEXPORT void JNICALL
Java_com_example_lammyopenglffmpegvideoplayer2_ILammyOpenglVideoPlayer_onSizeChanged(JNIEnv *env,
                                                                                 jclass type,
                                                                                 jfloat width,
                                                                                 jfloat height,
                                                                                 jlong nativeVideoPlayer)
{

    LammyOpenglVideoPlayer * lammyplayer =(LammyOpenglVideoPlayer *) nativeVideoPlayer;
    lammyplayer->openglVideoShow->SetViewPortSize(width, height);
//    lammyplayer->openglVideoShow->show(nullptr);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_lammyopenglffmpegvideoplayer2_ILammyOpenglVideoPlayer_onSurfaceDestroyed(JNIEnv *env,
                                                                                          jobject instance,
                                                                                          jlong nativeLammyVideoPlayer)
{

    LammyOpenglVideoPlayer * lammyplayer =(LammyOpenglVideoPlayer *) nativeLammyVideoPlayer;
    lammyplayer->onSurfaceDestroyed();

}

extern "C"
JNIEXPORT jboolean JNICALL
Java_com_example_lammyopenglffmpegvideoplayer2_ILammyOpenglVideoPlayer_onSurfaceCreated(JNIEnv *env,
                                                                                        jobject instance,
                                                                                        jobject surface,
                                                                                        jlong nativeLammyVideoPlayer)
{

    LammyOpenglVideoPlayer * lammyplayer =(LammyOpenglVideoPlayer *) nativeLammyVideoPlayer;
    ANativeWindow *win = ANativeWindow_fromSurface(env,surface);
    bool isPlay = lammyplayer->onSurfaceCreated(win);

    return  isPlay;

}