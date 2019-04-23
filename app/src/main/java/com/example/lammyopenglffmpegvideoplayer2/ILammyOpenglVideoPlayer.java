package com.example.lammyopenglffmpegvideoplayer2;

import android.content.res.AssetManager;
import android.view.Surface;

public class ILammyOpenglVideoPlayer {

    static {
        System.loadLibrary("lammyVideoPlayer");
    }

    private long nativeLammyPlayer;
    public ILammyOpenglVideoPlayer()
    {
        nativeLammyPlayer = initNativeVideoPlayer();
    }


    public long getNativePlayer(){
        return nativeLammyPlayer;
    }
    public native void InitView(Object surface, long nativeLammyVideoPlayer);
    public native void  onSurfaceDestroyed(long nativeLammyVideoPlayer);
    private native long initNativeVideoPlayer();
    public native void start(String videoPath, Surface surface, long nativeVideoPlayer);
    public native void pauseOrContinue(long nativeVideoPlayer );
    public native void seekTo(float progress,long nativeVideoPlayer );
    public native void close(long nativeVideoPlayer );
    public native void pause(long nativeVideoPlayer , int mode );
    // 若状态是暂停，则会播放，若不是暂停，则直接返回。
    public native void continuePlay(long nativeVideoPlayer);

    public static native void InitAssetManager(AssetManager am);
    public static native void InitOpenGL(long nativeVideoPlayer);
    public static native void OnViewportChanged(float width, float height,long nativeVideoPlayer);
    public static native void RenderOneFrame(long nativeVideoPlayer);



}
