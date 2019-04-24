package com.example.lammyopenglffmpegvideoplayer2;

import android.content.Context;
import android.content.res.AssetManager;
import android.opengl.GLSurfaceView;
import android.os.Environment;
import android.util.AttributeSet;
import android.util.Log;
import android.view.SurfaceHolder;
import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

public class LammyOpenglVideoPlayerView extends GLSurfaceView implements SurfaceHolder.Callback, GLSurfaceView.Renderer {//


    public static String videoPath = Environment.getExternalStorageDirectory().getPath() +"/ffmpeg/1080.mp4";
    public static String videoPath2 = "http://ivi.bupt.edu.cn/hls/cctv6hd.m3u8";

    private ILammyOpenglVideoPlayer lammyOpenglVideoPlayer;

    public LammyOpenglVideoPlayerView(Context context) {
        super(context);
        init();
    }

    public LammyOpenglVideoPlayerView(Context context, AttributeSet attrs) {
        super(context, attrs);
        init();
    }

    private void init(){
        lammyOpenglVideoPlayer = new ILammyOpenglVideoPlayer();
//        InitAssetManager(getContext().getAssets());
        setRenderer(this);//new LRenderer()
    }

    @Override
    public void surfaceCreated(SurfaceHolder var1)
    {
        lammyOpenglVideoPlayer.onSurfaceCreated(getHolder().getSurface(),lammyOpenglVideoPlayer.getNativePlayer());
//        lammyOpenglVideoPlayer.continuePlay(lammyOpenglVideoPlayer.getNativePlayer());

    }

    @Override
    public void surfaceChanged(SurfaceHolder var1, int var2, int var3, int var4)
    {

    }

    @Override
    public void surfaceDestroyed(SurfaceHolder var1)
    {
        lammyOpenglVideoPlayer.onSurfaceDestroyed(lammyOpenglVideoPlayer.getNativePlayer());
    }

    public void start(final String videoPath){
        if(getHolder().getSurface() != null)
        {
            lammyOpenglVideoPlayer.start(videoPath, getHolder().getSurface(), lammyOpenglVideoPlayer.getNativePlayer());
        }else
        {
            Log.e("lamm-java", "surface is null");
        }


    }

    public void pauseOrContinue(){
        lammyOpenglVideoPlayer.pauseOrContinue(lammyOpenglVideoPlayer.getNativePlayer());
    }

    public void pause(int mode)
    {
        lammyOpenglVideoPlayer.pause(lammyOpenglVideoPlayer.getNativePlayer(), mode);
    }

    public void continuePlay()
    {
        lammyOpenglVideoPlayer.continuePlay(lammyOpenglVideoPlayer.getNativePlayer());
    }
    public  void seekTo(float progress){
        lammyOpenglVideoPlayer.seekTo(progress, lammyOpenglVideoPlayer.getNativePlayer());
        requestRender();
    }

    public  void close(){
        lammyOpenglVideoPlayer.close(lammyOpenglVideoPlayer.getNativePlayer());
    }





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



}
