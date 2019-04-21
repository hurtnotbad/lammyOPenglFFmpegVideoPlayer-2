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
JNIEXPORT jlong JNICALL
Java_com_example_lammyopenglffmpegvideoplayer2_LammyOpenglVideoPlayerView_initNativeVideoPlayer(JNIEnv *env,
                                                                               jobject instance)
{
    LammyOpenglVideoPlayer *lammyplayer = new LammyOpenglVideoPlayer();
    return (jlong)(lammyplayer);
    // TODO

}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_lammyopenglffmpegvideoplayer2_LammyOpenglVideoPlayerView_play(JNIEnv *env,
                                                              jobject instance,
                                                              jstring videoPath_,
                                                              jobject surface,
                                                              jlong nativeVideoPlayer)
{
    const char *videoPath = env->GetStringUTFChars(videoPath_, 0);
    LammyOpenglVideoPlayer * lammyplayer =(LammyOpenglVideoPlayer *) nativeVideoPlayer;
//    ANativeWindow *win = ANativeWindow_fromSurface(env,surface);
//    lammyplayer->dataManager->win = win;
    lammyplayer->play(videoPath);

}
extern "C"
JNIEXPORT void JNICALL
Java_com_example_lammyopenglffmpegvideoplayer2_LammyOpenglVideoPlayerView_pauseOrContinue(JNIEnv *env,
                                                               jobject instance,
                                                               jlong nativeVideoPlayer)
{

    LammyOpenglVideoPlayer * lammyplayer =(LammyOpenglVideoPlayer *) nativeVideoPlayer;
    lammyplayer->pauseOrContinue();

}extern "C"
JNIEXPORT void JNICALL
Java_com_example_lammyopenglffmpegvideoplayer2_LammyOpenglVideoPlayerView_seekTo(JNIEnv *env,
                                                                jobject instance,
                                                                jfloat progress,
                                                                jlong nativeVideoPlayer)
{
    LammyOpenglVideoPlayer * lammyplayer =(LammyOpenglVideoPlayer *) nativeVideoPlayer;
    lammyplayer->seekTo(progress);

}
extern "C"
JNIEXPORT void JNICALL
Java_com_example_lammyopenglffmpegvideoplayer2_LammyOpenglVideoPlayerView_close(JNIEnv *env,
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
Java_com_example_lammyopenglffmpegvideoplayer2_LammyOpenglVideoPlayerView_InitAssetManager(JNIEnv *env,
                                                                                jclass type,
                                                                                jobject am)
{

    aAssetManager = AAssetManager_fromJava(env , am);

}


extern "C"
JNIEXPORT void JNICALL
Java_com_example_lammyopenglffmpegvideoplayer2_LammyOpenglVideoPlayerView_InitOpenGL(JNIEnv *env, jclass type ,
                                                                          jlong nativeVideoPlayer)
{


    LammyOpenglVideoPlayer *lammyplayer = (LammyOpenglVideoPlayer *) nativeVideoPlayer;
    lammyplayer->openglVideoShow->Init();

}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_lammyopenglffmpegvideoplayer2_LammyOpenglVideoPlayerView_OnViewportChanged(JNIEnv *env,
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
Java_com_example_lammyopenglffmpegvideoplayer2_LammyOpenglVideoPlayerView_RenderOneFrame(JNIEnv *env,
                                                                              jclass type,
                                                                              jlong nativeVideoPlayer)
{

//    LammyOpenglVideoPlayer * lammyplayer =(LammyOpenglVideoPlayer *) nativeVideoPlayer;
//    lammyplayer->openglVideoShow->Draw(lammyplayer);

}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_lammyopenglffmpegvideoplayer2_LammyOpenglVideoPlayerView_InitView(JNIEnv *env,
                                                                                   jobject instance,
                                                                                   jobject surface,
                                                                                   jlong nativeLammyVideoPlayer)
{

    LammyOpenglVideoPlayer * lammyplayer =(LammyOpenglVideoPlayer *) nativeLammyVideoPlayer;
    ANativeWindow *win = ANativeWindow_fromSurface(env,surface);
    lammyplayer->dataManager->win = win;
    lammyplayer->startVideo();
}extern "C"
JNIEXPORT void JNICALL
Java_com_example_lammyopenglffmpegvideoplayer2_LammyOpenglVideoPlayerView_onSurfaceDestroyed(JNIEnv *env,
                                                                                             jobject instance,
                                                                                             jlong nativeLammyVideoPlayer)
{

    LammyOpenglVideoPlayer * lammyplayer =(LammyOpenglVideoPlayer *) nativeLammyVideoPlayer;
    lammyplayer->stopVideo();

}