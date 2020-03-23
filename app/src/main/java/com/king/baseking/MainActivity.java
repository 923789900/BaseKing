package com.king.baseking;

import androidx.appcompat.app.AppCompatActivity;

import android.app.Application;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

public class MainActivity extends AppCompatActivity {

    // Used to load the 'native-lib' library on application startup.

    static
    {
        try
        {
            System.loadLibrary("BaseKing");
        }catch (Throwable e)
        {
            e.printStackTrace();
        }
    }


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        Button tv = findViewById(R.id.test);
        tv.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                try
                {
                    System.loadLibrary("BaseKing");
                }catch (Throwable e)
                {
                    e.printStackTrace();
                }
            }
        });
    }

}
