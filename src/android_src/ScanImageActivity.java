package com.example.sunriz.list_1;

import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Matrix;
import android.media.ExifInterface;
import android.net.Uri;
import android.os.Build;
import android.os.Bundle;
import android.support.annotation.RequiresApi;
import android.support.v7.app.AppCompatActivity;
import android.view.View;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.TextView;

import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.net.InetAddress;
import java.net.Socket;
import java.net.SocketException;
import java.util.LinkedList;

/**
 * Created by sunriz on 18-5-26.
 */

public class ScanImageActivity extends AppCompatActivity {

    private ImageView imageView;
    private TextView info_text;
    private Button button;
    private Uri imageUri;
    private Bitmap bitmap;

    @RequiresApi(api = Build.VERSION_CODES.N)
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.scan_image_layout);

        imageView = (ImageView)findViewById(R.id.imageView_scan);
        info_text = (TextView)findViewById(R.id.textView_info_scan);
        button = (Button)findViewById(R.id.button_confirm_scan);

        imageUri = Uri.parse(getIntent().getStringExtra("uri"));
        try
        {
            Bitmap bitmap0 = BitmapFactory.decodeStream(getContentResolver()
                    .openInputStream(imageUri));
            int degree = getBitmapDegree();
            bitmap = rotateBitmapByDegree(bitmap0,degree);
            imageView.setImageBitmap(bitmap);
        }catch (Exception e)
        {
            info_text.setText(e.toString());
        }

        button.setOnClickListener(new View.OnClickListener(){

            @Override
            public void onClick(View view) {
                String host="139.199.2.139";
                int port=8000;
                new ThreadNetBitmap_ScanForResults(host,port).start();
            }
        });

    }

    class ThreadNetBitmap_ScanForResults extends Thread {

        private Socket socket_fd;
        private String host;
        private int port;

        ThreadNetBitmap_ScanForResults(String host,int port) {
            this.host = host;
            this.port = port;
        }

        public void run() {
            try {

                socket_fd = new Socket();
                try {
                    InetAddress host_addr = InetAddress.getByName(host);
                    socket_fd = new Socket(host_addr,port);
                }catch (SocketException e)
                {
                    info_text.setText(e.toString());
                }

                try
                {
                    ByteArrayOutputStream bao = new ByteArrayOutputStream();
                    bitmap.compress(Bitmap.CompressFormat.JPEG,50,bao);
                    byte[] datas = bao.toByteArray();

                    DataOutputStream socket_out = new DataOutputStream(socket_fd.getOutputStream());
                    DataInputStream socket_in = new DataInputStream(socket_fd.getInputStream()) ;
                    DataInputStream src_ss = new DataInputStream(new ByteArrayInputStream(datas));

                    byte [] temp = new byte[1024];
                    int length = 0;
                    info_text.setText("sending image,please wait...");
                    int state;
                    state = 2; //state
                    socket_out.writeByte(state);

                    while((length=src_ss.read(temp,0,temp.length))>0)
                    {
                        socket_out.write(temp,0,length);
                        socket_out.flush();
                    }

                    socket_fd.shutdownOutput();
                    src_ss.close();

                    info_text.setText("send succeed,waiting for recognizing...");

                    byte[] student_temp = new byte[100];
                    for(int i=0;i<student_temp.length;i++)
                    {
                        student_temp[i] = 0;
                    }

/*
                    LinkedList<test_Data> test_list = new LinkedList<test_Data>();
                    test_Data student = new test_Data();
                    student.name = "huge";
                    student.id = "189712973";
                    test_list.add(student);

                   d test_Data student2 = new test_Data();
                    student2.name = "liushishi";
                    student2.id = "54309880";
                    test_list.add(student2);
*/
                    LinkedList<ResultData> result_list = new LinkedList<ResultData>();
                    byte[] str_temp = new byte[50];
                    for(int i=0;i<str_temp.length;i++)
                    {
                        str_temp[i] = 0;
                    }
                    int i=0;

                    while((length = socket_in.read(student_temp,0,56))>0)
                    {
                            ResultData student = new ResultData();
                            System.arraycopy(student_temp, 0, str_temp, 0, 20);
                            student.name = new String(str_temp);
                            System.arraycopy(student_temp, 20, str_temp, 0, 20);
                            student.id = new String(str_temp);
                            System.arraycopy(student_temp, 40, student.rect, 0, 16);
                            result_list.add(student);
                            i++;
                            CharSequence cs = String.valueOf(i);
                            info_text.setText(cs);
                    }

                    //由server主动关闭
                    socket_fd.close();//使用序列化intent传递对象
                    Intent intent = new Intent(ScanImageActivity.this,List.class);
                    List.result_list = result_list;
                    List.bitmap = bitmap;
                    ScanImageActivity.this.startActivity(intent);

                }catch (Exception e)
                {
                    info_text.setText(e.toString());
                }

            }catch(Exception e) {
                System.out.println(e);
                info_text.setText(e.toString());
            }
        }
    }

    @RequiresApi(api = Build.VERSION_CODES.N)
    private int getBitmapDegree() {
        int degree = 0;
        try {
            // 从指定路径下读取图片，并获取其EXIF信息
            ExifInterface exifInterface = new ExifInterface(getContentResolver().openInputStream(imageUri));
            // 获取图片的旋转信息
            int orientation = exifInterface.getAttributeInt(ExifInterface.TAG_ORIENTATION,
                    ExifInterface.ORIENTATION_NORMAL);
            switch (orientation) {
                case ExifInterface.ORIENTATION_ROTATE_90:
                    degree = 90;
                    break;
                case ExifInterface.ORIENTATION_ROTATE_180:
                    degree = 180;
                    break;
                case ExifInterface.ORIENTATION_ROTATE_270:
                    degree = 270;
                    break;
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
        return degree;
    }

    private Bitmap rotateBitmapByDegree(Bitmap bm,int degree) {
        Bitmap returnBm = null;

        // 根据旋转角度，生成旋转矩阵
        Matrix matrix = new Matrix();
        matrix.postRotate(degree);
        try {
            // 将原始图片按照旋转矩阵进行旋转，并得到新的图片
            returnBm = Bitmap.createBitmap(bm, 0, 0, bm.getWidth(), bm.getHeight(), matrix, true);
        } catch (OutOfMemoryError e) {
        }
        if (returnBm == null) {
            returnBm = bm;
        }
        if (bm != returnBm) {
            bm.recycle();
        }
        return returnBm;
    }
}
