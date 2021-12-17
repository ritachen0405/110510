package com.example.test_1;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

import com.loopj.android.http.AsyncHttpClient;
import com.loopj.android.http.AsyncHttpResponseHandler;

import cz.msebera.android.httpclient.Header;

public class water_q extends AppCompatActivity implements View.OnClickListener{

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_water_q);

        Button btnGet = (Button)findViewById(R.id.button);
        assert btnGet != null;
        btnGet.setOnClickListener(this);
    }
    public void onClick(final View v){
        v.setEnabled(false);
        AsyncHttpClient client = new AsyncHttpClient();
        TextView qtxt = (TextView) findViewById(R.id.dirty_water2);//濁度
        TextView wtxt= (TextView) findViewById(R.id.dirty_water);//溫度
        TextView phtxt=(TextView) findViewById(R.id.dirty_water3);//PH
        client.get("https://api.thingspeak.com/channels/1377526/fields/2.json?api_key=ZNCOHBASW59GMTA4&results=1", new AsyncHttpResponseHandler() {
            @Override
            public void onSuccess(int statusCode, Header[] headers, byte[] responseBody) {
                if (responseBody != null) {
                    String s= s_openFileToString(responseBody);
                    assert qtxt != null;
                    qtxt.setText(s);
                }
                v.setEnabled(true);
            }

            @Override
            public void onFailure(int statusCode, Header[] headers, byte[] responseBody, Throwable error) {
                v.setEnabled(true);
            }
        });
        client.get("https://api.thingspeak.com/channels/1377526/fields/1.json?api_key=ZNCOHBASW59GMTA4&results=1", new AsyncHttpResponseHandler() {
            @Override
            public void onSuccess(int statusCode, Header[] headers, byte[] responseBody) {
                if (responseBody != null) {
                    String s= openFileToString(responseBody);
                    assert wtxt != null;
                    wtxt.setText(s);
                }
                v.setEnabled(true);
            }

            @Override
            public void onFailure(int statusCode, Header[] headers, byte[] responseBody, Throwable error) {
                v.setEnabled(true);
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

}