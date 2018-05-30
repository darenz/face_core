package com.example.sunriz.list_1;

import android.graphics.Bitmap;
import android.graphics.drawable.Drawable;
import android.net.Uri;
import android.os.Build;
import android.os.Bundle;
import android.support.annotation.RequiresApi;
import android.support.v7.app.AppCompatActivity;
import android.view.View;
import android.widget.ImageView;
import android.widget.ListView;
import android.widget.SimpleAdapter;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.LinkedList;
import java.util.Map;

public class List extends AppCompatActivity {


    public static LinkedList<ResultData> result_list;
    public static Uri imageUri;
    public static Bitmap bitmap;


   // public static LinkedList<test_Data> result_list;


   /*  private int[] bmp_list = {
            R.drawable.liushishi,
            R.drawable.huojianhua
    };
*/

    @RequiresApi(api = Build.VERSION_CODES.N)
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.listactivity_layout);
        //imageUri = (Uri)getIntent().getSerializableExtra("uri");
/*
        try{
            Bitmap bitmap0 = BitmapFactory.decodeStream(getContentResolver()
                    .openInputStream(imageUri));
            int degree = getBitmapDegree();
            bitmap = rotateBitmapByDegree(bitmap0,degree);

        }catch (Exception e)
        {
            System.out.println(e.toString());
        }
*/
        String[] names = new String[result_list.size()];
        String[] IDs = new String[result_list.size()];
        Bitmap [] bmp_list = new Bitmap[result_list.size()];
        Drawable [] drawables = new Drawable[result_list.size()];

        for(int i=0;i<result_list.size();i++)
        {
            ResultData student = result_list.get(i);
            names[i] = student.name.toString();
            IDs[i] = student.id.toString();
          //  drawables[i] = new BitmapDrawable(Bitmap.createBitmap(bitmap,100,100,100,100));

            bmp_list[i] = Bitmap.createBitmap(bitmap,student.getIntX(),student.getIntY(),student.getIntWidth(),student.getIntHeight());
         //   bmp_list[i] = bitmap;
        }

/*
        for(int i=0;i<result_list.size();i++)
        {
            test_Data student = result_list.get(i);
            names[i] = student.name;
            IDs[i] = student.id;

        }
*/


        //每个数据项对应一个Map，from表示的是Map中key的数组
        String[] from = {"name", "ID", "icon_head"};

        //数据项Map中的每个key都在layout中有对应的View，
        //to表示数据项对应的View的ID数组
        int[] to = {R.id.name, R.id.ID, R.id.icon_head};

        java.util.List<Map<String, Object>> list = new ArrayList<>();
        for (int i = 0; i < names.length; i++)
        {
            HashMap<String, Object> map = new HashMap<>();
            map.put("name", names[i]);
            map.put("ID", IDs[i]);
            map.put("icon_head", bmp_list[i]);
            list.add(map);
        }

        //R.layout.item表示数据项UI所对应的layout文件
        SimpleAdapter adapter = new SimpleAdapter(this, list, R.layout.student_layout, from, to);

        adapter.setViewBinder(new SimpleAdapter.ViewBinder() {
            @Override
            public boolean setViewValue(View view, Object o, String s) {

                    // TODO Auto-generated method stub
                    if(view instanceof ImageView && o instanceof Bitmap){
                        ImageView i = (ImageView)view;
                        i.setImageBitmap((Bitmap) o);
                        return true;
                    }
                    return false;
            }
        });

        ListView listView = (ListView) findViewById(R.id.student_list);
        listView.setAdapter(adapter);
    }
}
