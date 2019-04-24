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

    public  void  onSurfaceDestroyed(){
        onSurfaceDestroyed( nativeLammyPlayer);
    }
    public  void  onSurfaceCreated(Surface surface)
    {
        onSurfaceCreated(surface,nativeLammyPlayer);
    }

    public void start(String videoPath, Surface surface)
    {
        start(videoPath,surface,nativeLammyPlayer);
    }

    public  void pauseOrContinue()
    {
        pauseOrContinue(nativeLammyPlayer);
    }

    public  void seekTo(float progress)
    {
        seekTo(progress, nativeLammyPlayer);
    }
    public  void close( )
    {
        close(nativeLammyPlayer);
    }

    public   void onSizeChanged(float width, float height)
    {
        onSizeChanged(width,height,nativeLammyPlayer);
    }



    private native void  onSurfaceDestroyed(long nativeLammyVideoPlayer);
    private native void  onSurfaceCreated(Surface surface,long nativeLammyVideoPlayer);
    private native long initNativeVideoPlayer();
    private native void start(String videoPath, Surface surface, long nativeVideoPlayer);
    private native void pauseOrContinue(long nativeVideoPlayer );
    private native void seekTo(float progress,long nativeVideoPlayer );
    private native void close(long nativeVideoPlayer );
    public static native void InitAssetManager(AssetManager am);
    private  native void onSizeChanged(float width, float height,long nativeVideoPlayer);



}
