package com.example.lammyopenglffmpegvideoplayer2;

import android.Manifest;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.os.Build;
import android.support.annotation.NonNull;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.Window;
import android.view.WindowManager;

public class MainActivity extends AppCompatActivity {

    private int permissionRequestCode = 1;
    private String permissions[] = new String[]{
            Manifest.permission.WRITE_EXTERNAL_STORAGE,
            Manifest.permission.READ_EXTERNAL_STORAGE,
            Manifest.permission.CAMERA,
            Manifest.permission.INTERNET

    };



    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        requestPermissions();
    }



    private void onPermissionGranted(){
        Intent intent = new Intent(getApplicationContext(),com.example.lammyopenglffmpegvideoplayer2.VideoPlayerActivity.class);
        startActivity(intent);
        finish();
    }
    private void requestPermissions(){
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {
            for(int i =0; i < permissions.length ; i ++) {

                if( checkSelfPermission(permissions[0]) == PackageManager.PERMISSION_DENIED){
                    requestPermissions(permissions ,permissionRequestCode );
                    return;
                    //               Toast.makeText(this , "请打开"+permissions[0]+"权限",Toast.LENGTH_LONG).show();
                }
            }
            onPermissionGranted();
        }else{
            onPermissionGranted();
        }
    }

    @Override
    public void onRequestPermissionsResult(int requestCode, @NonNull String[] permissions, @NonNull int[] grantResults) {

        if(requestCode ==permissionRequestCode ){

            for(int result:grantResults){
                if(result == PackageManager.PERMISSION_DENIED){
                    return;
                }
            }
            onPermissionGranted();

        }
    }
}
