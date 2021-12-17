package com.example.test_1;

import androidx.appcompat.app.AppCompatActivity;

import android.content.res.Resources;
import android.os.Bundle;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.Spinner;
import android.widget.TextView;

import com.loopj.android.http.AsyncHttpClient;
import com.loopj.android.http.AsyncHttpResponseHandler;

import cz.msebera.android.httpclient.Header;

public class feeding extends AppCompatActivity implements View.OnClickListener{

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_feeding);
        Button btnGet = (Button)findViewById(R.id.button3);
        assert btnGet != null;
        btnGet.setOnClickListener(this);
//        Resources res =getResources();
//        String[] time;
//        time = res.getStringArray(R.array.province1);
//        final ArrayAdapter<String> adapter = new ArrayAdapter<String>(this,android.R.layout.simple_list_item_1,time);//建立Arrayadapter介面卡
//        Spinner spinner = (Spinner) findViewById(R.id.spinner);
//        spinner.setAdapter(adapter);
//        spinner.setOnItemSelectedListener(new AdapterView.OnItemSelectedListener() {//通過此方法為下拉列表設定點選事件
//            @Override
//            public void onItemSelected(AdapterView<?> adapterView, View view, int i, long l) {
////                view.setEnabled(false);
//                String text= spinner.getItemAtPosition(i).toString();
//                AsyncHttpClient client = new AsyncHttpClient();
//                String getUrl;
//                getUrl="https://api.thingspeak.com/update?api_key=6QIBWXEW1U6DKJ37&field2="+text;
//                client.get(getUrl, new AsyncHttpResponseHandler() {
//                    @Override
//                    public void onSuccess(int statusCode, Header[] headers, byte[] responseBody) {
//                        if (responseBody != null) {
//
//                        }
////                        view.setEnabled(true);
//                    }
//
//                    @Override
//                    public void onFailure(int statusCode, Header[] headers, byte[] responseBody, Throwable error) {
////                        view.setEnabled(true);
//                    }
//                });
////                Toast.makeText(MainActivity.this,text, Toast.LENGTH_SHORT).show();
//            }
//            public void onNothingSelected(AdapterView<?> adapterView) {
//            }
//        });
    }
    @Override
    public void onClick(View view) {
        String text="";
        Spinner sp = findViewById(R.id.spinner1);
        text= sp.toString();
        AsyncHttpClient client = new AsyncHttpClient();
        String getUrl;
        getUrl="https://api.thingspeak.com/update?api_key=6QIBWXEW1U6DKJ37&field2="+text;
        client.get(getUrl, new AsyncHttpResponseHandler() {
            @Override
            public void onSuccess(int statusCode, Header[] headers, byte[] responseBody) {
                if (responseBody != null) {
                    TextView show = findViewById(R.id.show);
                    show.setText("Done");
                }
//                        view.setEnabled(true);
            }

            @Override
            public void onFailure(int statusCode, Header[] headers, byte[] responseBody, Throwable error) {
//                        view.setEnabled(true);
            }
        });
    }

}