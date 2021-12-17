package com.example.test_1;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.ImageButton;

public class MainActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        ImageButton btn_height = (ImageButton) findViewById(R.id.btn_height);
        btn_height.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Intent intentquality = new Intent();
                intentquality.setClass(MainActivity.this,water_h.class);
                startActivity(intentquality);

            }

        });
        ImageButton btn_quality = (ImageButton) findViewById(R.id.btn_quality);
        btn_quality.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Intent intentquality = new Intent();
                intentquality.setClass(MainActivity.this,water_q.class);
                startActivity(intentquality);

            }

        });
        ImageButton btn_water_change = (ImageButton) findViewById(R.id.btn_water_changing);
        btn_water_change.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Intent intentquality = new Intent();
                intentquality.setClass(MainActivity.this,water_change.class);
                startActivity(intentquality);

            }

        });
        ImageButton btn_feeding = (ImageButton) findViewById(R.id.btn_feeding);
        btn_feeding.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Intent intentquality = new Intent();
                intentquality.setClass(MainActivity.this,feeding.class);
                startActivity(intentquality);

            }

        });
    }
}