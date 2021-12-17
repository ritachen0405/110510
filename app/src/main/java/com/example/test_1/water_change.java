package com.example.test_1;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.ImageButton;
import android.widget.Spinner;
import android.widget.TextView;

import com.loopj.android.http.AsyncHttpClient;
import com.loopj.android.http.AsyncHttpResponseHandler;

import cz.msebera.android.httpclient.Header;

public class water_change extends AppCompatActivity{

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_water_change);
        Button btn_re =  findViewById(R.id.button2);
        btn_re.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                view.setEnabled(false);
                AsyncHttpClient client = new AsyncHttpClient();
                TextView qtxt = (TextView) findViewById(R.id.TurbidityTextNumber);//濁度
                TextView phtxt=(TextView) findViewById(R.id.PHTextNumber2);//PH
                client.get("https://api.thingspeak.com/channels/1377526/fields/2.json?api_key=ZNCOHBASW59GMTA4&results=1", new AsyncHttpResponseHandler() {
                    @Override
                    public void onSuccess(int statusCode, Header[] headers, byte[] responseBody) {
                        if (responseBody != null) {
                            String s= s_openFileToString(responseBody);
                            assert qtxt != null;
                            qtxt.setText(s);
                        }
                        view.setEnabled(true);
                    }

                    @Override
                    public void onFailure(int statusCode, Header[] headers, byte[] responseBody, Throwable error) {
                        view.setEnabled(true);
                    }
                });
                client.get("https://api.thingspeak.com/channels/1377526/fields/3.json?api_key=ZNCOHBASW59GMTA4&results=1", new AsyncHttpResponseHandler() {
                    @Override
                    public void onSuccess(int statusCode, Header[] headers, byte[] responseBody) {
                        if (responseBody != null) {
                            String s= s_openFileToString(responseBody);
                            assert phtxt != null;
                            phtxt.setText(s);
                        }
                        view.setEnabled(true);
                    }

                    @Override
                    public void onFailure(int statusCode, Header[] headers, byte[] responseBody, Throwable error) {
                        view.setEnabled(true);
                    }
                });
            }

        });
        Button btn_waterchange =  findViewById(R.id.wc_btn);
        btn_waterchange.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                view.setEnabled(false);
                String text="";
                Spinner sp = findViewById(R.id.spinner);
                text= sp.toString();
                AsyncHttpClient client = new AsyncHttpClient();
                String getUrl;
                getUrl="https://api.thingspeak.com/update?api_key=6QIBWXEW1U6DKJ37&field1="+text;
                client.get(getUrl, new AsyncHttpResponseHandler() {
                    @Override
                    public void onSuccess(int statusCode, Header[] headers, byte[] responseBody) {
                        if (responseBody != null) {
                            TextView tv = findViewById(R.id.textView);
                            tv.setText("Done");
                        }
//                        view.setEnabled(true);
                    }

                    @Override
                    public void onFailure(int statusCode, Header[] headers, byte[] responseBody, Throwable error) {
//                        view.setEnabled(true);
                    }
            });
    }});


}
public String s_openFileToString(byte[] _bytes)
    {
        String file_string = "";

        for(int i = 0; i < _bytes.length; i++)
        {
            if(309<=i&&i<=311){
                file_string += (char)_bytes[i];
            }else {continue;}

        }

        return file_string;
    }
    public String openFileToString(byte[] _bytes)
    {
        String file_string = "";

        for(int i = 0; i < _bytes.length; i++)
        {
            if(309<=i&&i<=312){
                file_string += (char)_bytes[i];
            }else {continue;}

        }

        return file_string;
    }
}