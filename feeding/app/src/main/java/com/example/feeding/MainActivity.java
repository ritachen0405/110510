package com.example.feeding;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.ImageButton;
import android.widget.TextView;

import androidx.appcompat.app.AppCompatActivity;

public class MainActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        float a =0;//水位
        float b =0;//濁度
        float c =0;//溫度
        float d =0;//ph值
        super.onCreate(savedInstanceState);
        setContentView(R.layout.feeding_main);
        TextView vew_h = findViewById(water_height);
        vew_h.setText(Double.toString(a));
        TextView vew_d = findViewById(dirty_water3);
        vew_d.setText(Double.toString(b));
        TextView vew_temp = findViewById(dirty_water2);
        vew_temp.setText(Double.toString(c));
        TextView vew_ph = findViewById(dirty_water);
        vew_ph.setText(Double.toString(d));
        TextView vew_d1 = findViewById(TurbidityTextNumber);
        vew_d1.setText(Double.toString(b));
        TextView vew_ph2 = findViewById(PHTextNumber2);
        vew_ph2.setText(Double.toString(d));
        TextView vew_d3 = findViewById(spinner);
        vew_d3.setText(Double.toString(b));

    }

private ImageButton btn_quality;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.function);

        btn_quality = (ImageButton) findViewById(R.id.btn_quality);
        btn_quality.setOnClickListener(new View.OnClickListener() {

            @Override
            public void onClick(View view) {
                Intent intentquality = new Intent();
                intentquality.setClass(MainActivity.this,water_quality.class);
                startActivity(intentquality);

            }

        });
    }
}