package com.example.sunriz.list_1;

import java.io.Serializable;

/**
 * Created by sunriz on 18-5-25.
 */

public class ResultData implements Serializable {

    public String name;
    public String id;
    public byte[] rect;

    ResultData()
    {
        rect = new byte[16];

    }

    private int x;
    private int y;
    private int width;
    private int height;

    public int getIntX()
    {
        x=0;
        byte[] temp = new byte[4];
        for(int i=0;i<4;i++)
        {
            temp[i] = rect[i];
        }

        for (int i = 0; i < 4; i++) {
            int shift= i * 8;
            x +=(temp[i] & 0x000000FF) << shift;
        }
        return x;
    }

    public int getIntY()
    {
        y=0;
        byte[] temp = new byte[4];
        for(int i=0;i<4;i++)
        {
            temp[i] = rect[i+4];
        }

        for (int i = 0; i < 4; i++) {
            int shift=  i * 8;
            y +=(temp[i] & 0x000000FF) << shift;
        }
        return y;
    }

    public int getIntWidth()
    {
        width=0;
        byte[] temp = new byte[4];
        for(int i=0;i<4;i++)
        {
            temp[i] = rect[i+8];
        }
        for (int i = 0; i < 4; i++) {
            int shift=  i * 8;
            width +=(temp[i] & 0x000000FF) << shift;
        }
        return width;
    }

    public int getIntHeight()
    {
        height=0;
        byte[] temp = new byte[4];
        for(int i=0;i<4;i++)
        {
            temp[i] = rect[i+12];
        }
        for (int i = 0; i < 4; i++) {
            int shift=  i * 8;
            height +=(temp[i] & 0x000000FF) << shift;
        }
        return height;
    }
}
