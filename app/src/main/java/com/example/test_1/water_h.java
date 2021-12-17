package com.example.test_1;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.os.Handler;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

import com.loopj.android.http.AsyncHttpClient;
import com.loopj.android.http.AsyncHttpResponseHandler;

import org.apache.http.HttpEntity;
import org.apache.http.HttpResponse;
import org.apache.http.client.HttpClient;
import org.apache.http.client.methods.HttpGet;
import org.apache.http.impl.client.DefaultHttpClient;
import org.apache.http.util.EntityUtils;

import java.io.IOException;
import java.net.URL;

import cz.msebera.android.httpclient.Header;

public class water_h extends AppCompatActivity implements View.OnClickListener {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_water_h);
        Button btnGet = (Button)findViewById(R.id.btnGet);
        assert btnGet != null;
        btnGet.setOnClickListener(this);
        //btnGet.callOnClick();
        //onClick(View v);

    }

    @Override
    public void onClick(final View v){
        v.setEnabled(false);
        TextView txtReponse = (TextView) findViewById(R.id.water_height);
        AsyncHttpClient client = new AsyncHttpClient();


        client.get("https://api.thingspeak.com/channels/1377526/fields/4.json?api_key=ZNCOHBASW59GMTA4&results=1", new AsyncHttpResponseHandler() {
            @Override
            public void onSuccess(int statusCode, Header[] headers, byte[] responseBody) {
                if (responseBody != null) {
                    String s = openFileToString(responseBody);
                    assert txtReponse != null;
                    txtReponse.setText(s);
                }
                v.setEnabled(true);
            }

            @Override
            public void onFailure(int statusCode, Header[] headers, byte[] responseBody, Throwable error) {
                v.setEnabled(true);
            }
        });

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

//public class water_h extends AppCompatActivity {
//    private URL url;
//    Http_Get HG;
//    private String getUrl = "http://api.thingspeak.com/channels/1503956/fields/1.json?results=2";//"https://api.thingspeak.com/channels/1377526/feeds.json?api_key=ZNCOHBASW59GMTA4&results=1";
//    static Handler handler; //宣告成static讓service可以直接使用
//    @Override
//    protected void onCreate(Bundle savedInstanceState) {
//        super.onCreate(savedInstanceState);
//        setContentView(R.layout.activity_water_h);
//        float  height=0;
////        String[] data=new String[30];
//        AsyncHttpClient client = new AsyncHttpClient();
//        client.get(getUrl, new AsyncHttpResponseHandler() {
//            @Override
//            public void onSuccess(int statusCode, Header[] headers, byte[] responseBody) {
//                TextView water_height = findViewById(R.id.water_height);
//                water_height.setText(new String(responseBody));
//            }
//
//            @Override
//            public void onFailure(int statusCode, Header[] headers, byte[] responseBody, Throwable error) {
//                TextView water_height = findViewById(R.id.water_height);
//                water_height.setText("fail");
//            }
//        });
//
////        DefaultHttpClient client = new DefaultHttpClient();//無例外閃退
////        HttpGet get = new HttpGet(getUrl);
////        try {
////            HttpResponse response = client.execute(get);
////            HttpEntity resEntity = response.getEntity();
////            data = EntityUtils.toString(resEntity);
////            TextView water_height = findViewById(R.id.water_height);
////            water_height.setText(data);
////        } catch (IOException e) {
////            e.printStackTrace();
////        }
//
////        HG = new Http_Get();
////        HG.Get(getUrl);
////        data = HG.toString();
////        data="400";
//
//    }
//}