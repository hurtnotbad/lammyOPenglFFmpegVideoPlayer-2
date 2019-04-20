package com.example.lammyopenglffmpegvideoplayer2;

import android.content.Context;
import android.content.res.AssetManager;
import android.opengl.GLSurfaceView;
import android.os.Environment;
import android.util.AttributeSet;
import android.util.Log;
import android.view.Surface;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

public class LammyOpenglVideoPlayerView extends GLSurfaceView {
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


    private void init(){
        nativeLammyVideoPlayer =  initNativeVideoPlayer();
        setEGLContextClientVersion(2);
        setRenderer(new LRenderer());
        setRenderMode(GLSurfaceView.RENDERMODE_WHEN_DIRTY);
        InitAssetManager(getContext().getAssets());

    }



    class LRenderer implements Renderer{

        @Override
        public void onSurfaceCreated(GL10 gl, EGLConfig config) {
            Log.e("lammy-java","onSurfaceCreated ......." );
            InitOpenGL(nativeLammyVideoPlayer);
            play(videoPath);
        }

        @Override
        public void onSurfaceChanged(GL10 gl, int width, int height) {
            OnViewportChanged(width , height,nativeLammyVideoPlayer);
        }

        @Override
        public void onDrawFrame(GL10 gl) {
            Log.e("lammy-java","onDrawFrame ......."+isPlaying );

            if(isPlaying){
                RenderOneFrame(nativeLammyVideoPlayer);
                LammyOpenglVideoPlayerView.this.requestRender();
            }

        }
    }

    private boolean isPlaying = false;
    private boolean isClose = false;

    public boolean isPlaying(){
        return isPlaying;
    }
    public boolean isClose(){
      return isClose;
    }


    public void play(String videoPath){
        Surface surface =  getHolder().getSurface();
        if(surface != null&& nativeLammyVideoPlayer!= 0)
        {
            play(videoPath, surface, nativeLammyVideoPlayer);
            isPlaying =true;
            isClose = false;
        }
    }
    public void pauseOrContinue(){

            pause(nativeLammyVideoPlayer);
           isPlaying = !isPlaying;
            if (isPlaying) {
                requestRender();
            }

    }
    public  void seekTo(float progress){
        seekTo(progress, nativeLammyVideoPlayer );
        isPlaying = true;
        requestRender();
    }
    public void updateFrame(){
        Log.e("lammy-java","updateFrame ......." );
        requestRender();
    }
    public  void close(){
        close(nativeLammyVideoPlayer);isPlaying = false;
        isClose = true;
    }


    private native long initNativeVideoPlayer();
    private native void play(String videoPath, Surface surface, long nativeVideoPlayer);
    private native void pause(long nativeVideoPlayer );
    private native void seekTo(float progress,long nativeVideoPlayer );
    private native void close(long nativeVideoPlayer );

    public static native void InitAssetManager(AssetManager am);
    public static native void InitOpenGL(long nativeVideoPlayer);
    public static native void OnViewportChanged(float width, float height,long nativeVideoPlayer);
    public static native void RenderOneFrame(long nativeVideoPlayer);



}
