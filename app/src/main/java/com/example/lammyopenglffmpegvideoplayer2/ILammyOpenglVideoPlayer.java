package com.example.lammyopenglffmpegvideoplayer2;

import android.content.res.AssetManager;
import android.view.Surface;

public class ILammyOpenglVideoPlayer {

    static {
        System.loadLibrary("lammyVideoPlayer");
    }

    private long nativeLammyPlayer;
    private boolean isPlaying = false;
    private boolean isClosed = true;

    public ILammyOpenglVideoPlayer()
    {
        nativeLammyPlayer = initNativeVideoPlayer();
    }

    public boolean isPlaying()
    {
        return isPlaying;
    }

    public  void  onSurfaceDestroyed(){
        onSurfaceDestroyed( nativeLammyPlayer);
        isPlaying = false;
    }
    public  void  onSurfaceCreated(Surface surface)
    {
        if(onSurfaceCreated(surface,nativeLammyPlayer))
        {
            isPlaying = true;
        }else{
            isPlaying = false;
        }
    }

    public void start(String videoPath, Surface surface)
    {
        start(videoPath,surface,nativeLammyPlayer);
        isPlaying = true;
        isClosed = false;
    }

    public  void pauseOrContinue()
    {
        pauseOrContinue(nativeLammyPlayer);
        isPlaying = !isPlaying;
    }

    public  void seekTo(float progress)
    {
        seekTo(progress, nativeLammyPlayer);
        isPlaying = true;
    }
    public  void close( )
    {
        close(nativeLammyPlayer);
        isClosed = true;
    }

    public boolean isClosed()
    {
        return isClosed;
    }
    public   void onSizeChanged(float width, float height)
    {
        onSizeChanged(width,height,nativeLammyPlayer);
    }



    private native void  onSurfaceDestroyed(long nativeLammyVideoPlayer);
    private native boolean  onSurfaceCreated(Surface surface,long nativeLammyVideoPlayer);
    private native long initNativeVideoPlayer();
    private native void start(String videoPath, Surface surface, long nativeVideoPlayer);
    private native void pauseOrContinue(long nativeVideoPlayer );
    private native void seekTo(float progress,long nativeVideoPlayer );
    private native void close(long nativeVideoPlayer );
    public static native void InitAssetManager(AssetManager am);
    private  native void onSizeChanged(float width, float height,long nativeVideoPlayer);



}
