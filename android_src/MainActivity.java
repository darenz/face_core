package com.example.sunriz.list_1;

import android.app.Activity;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.graphics.Bitmap;
import android.net.Uri;
import android.os.Environment;
import android.provider.MediaStore;
import android.support.v4.app.ActivityCompat;
import android.support.v4.content.ContextCompat;
import android.support.v4.content.FileProvider;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

import java.io.*;


public class MainActivity extends AppCompatActivity {

    private Button mTakeImg_insert;
    private Button mTakeImg_scan;
    private Uri imageUri;
    public static final int TAKE_PHOTO_SCAN = 1;
    public static final int TAKE_PHOTO_INSERT = 2;

    private TextView nameText;
    private TextView idText;

    private Bitmap bitmap;

//permission codes
    private static final int CAMERA_OK = 100;
    private static final int REQUEST_EXTERNAL_STORAGE = 200;
    private static final int NET_OK = 300;

    private static String[] PERMISSIONS_STORAGE = {
            "android.permission.READ_EXTERNAL_STORAGE",
            "android.permission.WRITE_EXTERNAL_STORAGE" };

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);


        /////////////////////////////////////////take photo///////////////////////////////////
        mTakeImg_insert = (Button) findViewById(R.id.button_insert);
        mTakeImg_insert.setText("录入");
        mTakeImg_insert.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {

                   //摄像头权限
                if (ContextCompat.checkSelfPermission(MainActivity.this,
                        android.Manifest.permission.CAMERA) != PackageManager.PERMISSION_GRANTED) {
                    ActivityCompat.requestPermissions(MainActivity.this,
                            new String[]{android.Manifest.permission.CAMERA}, CAMERA_OK);

                } else {
                   //存储权限
                    if (verifyStoragePermissions(MainActivity.this) == PackageManager.PERMISSION_GRANTED) {
                        //网络权限
                        if (ContextCompat.checkSelfPermission(MainActivity.this,
                                android.Manifest.permission.INTERNET) != PackageManager.PERMISSION_GRANTED) {
                            ActivityCompat.requestPermissions(MainActivity.this,
                                    new String[]{android.Manifest.permission.INTERNET}, NET_OK);
                        } else {

                            //     SimpleDateFormat timeStampFormat = new SimpleDateFormat("yyyy_MM_dd_HH_mm_ss");

                            //      String filename = timeStampFormat.format(new Date());
                            String filename = "insert";
                            File outputImage = new File(Environment.getExternalStorageDirectory(),
                                    filename + ".jpg");
                            if (outputImage.exists()) {
                                outputImage.delete();
                            }
                            try {
                                outputImage.createNewFile();
                            } catch (IOException e) {
                                e.printStackTrace();
                            }

                          //  ContentValues values = new ContentValues(1);
                         //   values.put(MediaStore.Audio.Media.DATA, outputImage.getAbsolutePath());

                            //   imageUri = getContentResolver().insert(MediaStore.Images.Media.EXTERNAL_CONTENT_URI, values);

                            imageUri = FileProvider.getUriForFile(MainActivity.this, "com.example.sunriz.list_1.fileprovider", outputImage);

                            Intent intent = new Intent(MediaStore.ACTION_IMAGE_CAPTURE);
                            intent.putExtra(MediaStore.EXTRA_OUTPUT, imageUri);
                            startActivityForResult(intent, TAKE_PHOTO_INSERT);
                        }
                    }
                }
                }

        });

        mTakeImg_scan = (Button) findViewById(R.id.button_scan);
        mTakeImg_scan.setText("点名");
        mTakeImg_scan.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {

                //摄像头权限
                if (ContextCompat.checkSelfPermission(MainActivity.this,
                        android.Manifest.permission.CAMERA) != PackageManager.PERMISSION_GRANTED) {
                    ActivityCompat.requestPermissions(MainActivity.this,
                            new String[]{android.Manifest.permission.CAMERA}, CAMERA_OK);

                } else {
                    //存储权限
                    if (verifyStoragePermissions(MainActivity.this) == PackageManager.PERMISSION_GRANTED) {
                        //网络权限
                        if (ContextCompat.checkSelfPermission(MainActivity.this,
                                android.Manifest.permission.INTERNET) != PackageManager.PERMISSION_GRANTED) {
                            ActivityCompat.requestPermissions(MainActivity.this,
                                    new String[]{android.Manifest.permission.INTERNET}, NET_OK);
                        } else {

                            String filename = "scanAll";
                            File outputImage = new File(Environment.getExternalStorageDirectory(),
                                    filename + ".jpg");
                            if (outputImage.exists()) {
                                outputImage.delete();
                            }
                            try {
                                outputImage.createNewFile();
                            } catch (IOException e) {
                                e.printStackTrace();
                            }

                            imageUri = FileProvider.getUriForFile(MainActivity.this, "com.example.sunriz.list_1.fileprovider", outputImage);

                            Intent intent = new Intent(MediaStore.ACTION_IMAGE_CAPTURE);
                            intent.putExtra(MediaStore.EXTRA_OUTPUT, imageUri);
                            startActivityForResult(intent, TAKE_PHOTO_SCAN);
                        }
                    }


                }
            }

        });
    }

    protected void onActivityResult(int requestCode, int resultCode, Intent data) {

        super.onActivityResult(requestCode, resultCode, data);
        if(requestCode == TAKE_PHOTO_INSERT)
        {
            try {

                Intent intent = new Intent(MainActivity.this,InsertImageActivity.class);
                intent.putExtra("uri",imageUri.toString());
                startActivity(intent);

            } catch (Exception e) {
                e.printStackTrace();
            }
        }
        else if(requestCode == TAKE_PHOTO_SCAN)
        {
            try {

                Intent intent = new Intent(MainActivity.this,ScanImageActivity.class);
                intent.putExtra("uri",imageUri.toString());
                startActivity(intent);

            } catch (Exception e) {
                e.printStackTrace();
            }
        }
    }

    public static int verifyStoragePermissions(Activity activity) {

        try {
            //检测是否有写的权限
            int permission = ActivityCompat.checkSelfPermission(activity,
                    "android.permission.WRITE_EXTERNAL_STORAGE");
            if (permission != PackageManager.PERMISSION_GRANTED) {
                // 没有写的权限，去申请写的权限，会弹出对话框
                ActivityCompat.requestPermissions(activity, PERMISSIONS_STORAGE,REQUEST_EXTERNAL_STORAGE);
            }
        } catch (Exception e) {
            e.printStackTrace();
            System.out.println("storage permission denied");
        }
        return ActivityCompat.checkSelfPermission(activity,"android.permission.WRITE_EXTERNAL_STORAGE");
    }

}
