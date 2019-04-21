package com.example.lammyopenglffmpegvideoplayer2;

import android.content.Context;
import android.content.res.AssetManager;
import android.opengl.GLSurfaceView;
import android.os.Environment;
import android.util.AttributeSet;
import android.util.Log;
import android.view.Surface;
import android.view.SurfaceHolder;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

public class LammyOpenglVideoPlayerView extends GLSurfaceView implements SurfaceHolder.Callback, GLSurfaceView.Renderer {//
    static {
        System.loadLibrary("lammyVideoPlayer");
    }
    public static String videoPath = Environment.getExternalStorageDirectory().getPath() +"/ffmpeg/1080.mp4";
    public static String videoPath2 = "http://ivi.bupt.edu.cn/hls/cctv6hd.m3u8";
    private long nativeLammyVideoPlayer;

    public LammyOpenglVideoPlayerView(Context context) {
        super(context);
        init();
    }

    public LammyOpenglVideoPlayerView(Context context, AttributeSet attrs) {
        super(context, attrs);
        init();
    }

    @Override
    public void surfaceCreated(SurfaceHolder var1)
    {
        InitView(var1.getSurface());
        InitOpenGL(nativeLammyVideoPlayer);
    }

    @Override
    public void surfaceChanged(SurfaceHolder var1, int var2, int var3, int var4)
    {

    }

    @Override
    public void surfaceDestroyed(SurfaceHolder var1)
    {

    }




    private void init(){
        nativeLammyVideoPlayer =  initNativeVideoPlayer();
        InitAssetManager(getContext().getAssets());
        setRenderer(this);//new LRenderer()
//        setRenderMode(GLSurfaceView.RENDERMODE_WHEN_DIRTY);

    }

    public native void InitView(Object surface);

    @Override
    public void onSurfaceCreated(GL10 gl, EGLConfig config) {
        Log.e("lammy-java","onSurfaceCreated");

    }

    @Override
    public void onSurfaceChanged(GL10 gl, int width, int height) {
        Log.e("lammy-java","onSurfaceChanged");
    }

    @Override
    public void onDrawFrame(GL10 gl) {

        Log.e("lammy-java","onDrawFrame");
    }



    public void play(String videoPath){
        while(true){
            Surface surface =  getHolder().getSurface();
            if(surface != null&& nativeLammyVideoPlayer!= 0)
            {
                play(videoPath, surface, nativeLammyVideoPlayer);
                return;
            }
            try {
                Thread.sleep(10);
                continue;
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }

    }

    public void pauseOrContinue(){
        pauseOrContinue(nativeLammyVideoPlayer);
    }
    public  void seekTo(float progress){
        seekTo(progress, nativeLammyVideoPlayer );
        requestRender();
    }

    public  void close(){
        close(nativeLammyVideoPlayer);
    }


    private native long initNativeVideoPlayer();
    private native void play(String videoPath, Surface surface, long nativeVideoPlayer);
    private native void pauseOrContinue(long nativeVideoPlayer );
    private native void seekTo(float progress,long nativeVideoPlayer );
    private native void close(long nativeVideoPlayer );

    public static native void InitAssetManager(AssetManager am);
    public static native void InitOpenGL(long nativeVideoPlayer);
    public static native void OnViewportChanged(float width, float height,long nativeVideoPlayer);
    public static native void RenderOneFrame(long nativeVideoPlayer);



}