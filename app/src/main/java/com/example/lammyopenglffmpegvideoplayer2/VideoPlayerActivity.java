package com.example.lammyopenglffmpegvideoplayer2;

import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.SeekBar;

public class VideoPlayerActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_video_player);
        initView();
    }

    private LammyOpenglVideoPlayerView lammyOpenglVideoPlayerView;
    private Button pauseContinue_Bt;
    private SeekBar seek_bar;
    private void initView(){
        lammyOpenglVideoPlayerView = findViewById(R.id.lammy_video_view);
        pauseContinue_Bt = findViewById(R.id.pauseContinue_bt);
        seek_bar =findViewById(R.id.seek_bar);
        seek_bar.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener(){

            @Override
            public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {

            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {

            }

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {
                 int seekPos = seekBar.getProgress();
                float progress = ((float)seekPos)/100.0f;
                Log.e("seek", "seek bar = "+progress);
                lammyOpenglVideoPlayerView.seekTo(progress);
                pauseContinue_Bt.setText("pause");

            }
        });
    }



    public void pauseContinue(View view){
//        lammyOpenglVideoPlayerView.pauseOrContinue();
//        if(lammyOpenglVideoPlayerView.isPlaying()){
//            pauseContinue_Bt.setText("pause");
//        }else{
//            pauseContinue_Bt.setText("play");
//        }
        lammyOpenglVideoPlayerView.play(LammyOpenglVideoPlayerView.videoPath);
    }


    @Override
    public void onPause() {
        super.onPause();
//        Log.e("lammy-java","lammyOpenglVideoPlayer pauseORcontinue .......onPause" );
//        if(lammyOpenglVideoPlayerView.isPlaying())
//        {
//            lammyOpenglVideoPlayerView.pauseOrContinue();
//        }
    }

    @Override
    public void onBackPressed() {
        lammyOpenglVideoPlayerView.close();
        super.onBackPressed();

    }

    @Override
    protected void onResume() {
        super.onResume();
//        Log.e("lammy-java","lammyOpenglVideoPlayer pauseORcontinue .......onResume" );
//        new Thread(new Runnable() {
//            @Override
//            public void run() {
//                try {
//                    Thread.sleep(600);
//                    if(!lammyOpenglVideoPlayerView.isClose() && !lammyOpenglVideoPlayerView.isPlaying())
//                    {
//                        lammyOpenglVideoPlayerView.pauseOrContinue();
//                    }
//                } catch (InterruptedException e) {
//                    e.printStackTrace();
//                }
//
//            }
//        }).start();
//
    }

}
