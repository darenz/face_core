package com.example.sunriz.list_1;

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
import android.widget.EditText;
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

/**
 * Created by sunriz on 18-5-26.
 */

public class InsertImageActivity extends AppCompatActivity {


    private EditText nameText;
    private EditText idText;
    private Button button;
    private TextView info_text;

    private Uri imageUri;
    private ImageView imageView;
    private Bitmap bitmap;

    @RequiresApi(api = Build.VERSION_CODES.N)
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.insert_image_layout);

        imageView = (ImageView)findViewById(R.id.imageView_insert);
        nameText = (EditText)findViewById(R.id.editText_name);
        idText = (EditText)findViewById(R.id.editText_id);
        button = (Button)findViewById(R.id.button_comfirm_insert);
        info_text = (TextView)findViewById(R.id.textView_info_insert);

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

        button.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {

                String name = nameText.getText().toString();
                String id = idText.getText().toString();
                String host = "139.199.2.139";
                int port=8000;
                new ThreadNetBitmap_InsertStudent(host,port,name,id).start();

            }
        });

    }

    class ThreadNetBitmap_InsertStudent extends Thread {

        private Socket socket_fd;
        private String host;
        private int port;
        private String name;
        private String id;

        ThreadNetBitmap_InsertStudent(String host,int port,String name,String id) {
            this.host = host;
            this.port = port;
            this.name = name;
            this.id = id ;
        }

        public void run() {
            try {

                socket_fd = new Socket();
                try {
                    InetAddress host_addr = InetAddress.getByName(host);
                    socket_fd = new Socket(host_addr,port);
                }catch (SocketException e)
                {
                    System.out.println(e.toString());
                }

                try
                {
                    ByteArrayOutputStream bao = new ByteArrayOutputStream();
                    bitmap.compress(Bitmap.CompressFormat.JPEG,50,bao);
                    byte[] datas = bao.toByteArray();

                    DataOutputStream socket_out = new DataOutputStream(socket_fd.getOutputStream());
                    DataInputStream src_ss = new DataInputStream(new ByteArrayInputStream(datas));

                    byte [] temp = new byte[1024];
                    int length = 0;
                    info_text.setText("sending image,please wait...");

                  int state = 1;
                    socket_out.writeByte(state);

                    socket_out.writeByte(name.length());
                    socket_out.writeBytes(name);
                    socket_out.flush();

                    socket_out.writeByte(id.length());
                    socket_out.writeBytes(id);
                    socket_out.flush();

                    while((length=src_ss.read(temp,0,temp.length))>0)
                    {
                        socket_out.write(temp,0,length);
                        socket_out.flush();
                    }
                    socket_fd.shutdownOutput();
                    src_ss.close();

                    info_text.setText("send succeed");
                    state=0;

                    DataInputStream socket_in = new DataInputStream(socket_fd.getInputStream());
                    state = socket_in.readByte();
                    if(state==1) //succeed
                        info_text.setText("insert information succeed");
                    else
                        info_text.setText("insert information failed,check existence");

                    socket_fd.close();

                }catch (Exception e)
                {
                    info_text.setText(e.toString());
                }

            }catch(Exception e) {
                System.out.println(e);
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
