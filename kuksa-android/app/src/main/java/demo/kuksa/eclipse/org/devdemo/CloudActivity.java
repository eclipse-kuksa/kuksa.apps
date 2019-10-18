package demo.kuksa.eclipse.org.devdemo;

import android.os.AsyncTask;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.webkit.WebView;

import java.net.URL;

public class CloudActivity extends AppCompatActivity {


    WebView grafanaView;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_cloud);
        //super.onCreate(savedInstanceState);


        grafanaView = findViewById(R.id.grafView);
        grafanaView.getSettings().setLoadWithOverviewMode(true);
        grafanaView.getSettings().setUseWideViewPort(true);
        /*try {
            new GrafTask().execute(new URL("https://developer.mozilla.org"));
        } catch (MalformedURLException e) {
            Log.e("kuksa-dev","URL not ok!");
        }*/
        String grafanaIP = getIntent().getStringExtra("GRAFANA_IP");
        grafanaView.loadUrl("http://"+grafanaIP+":3000");


    }

    private class GrafTask extends AsyncTask<URL, Integer, Long> {


        @Override
        protected Long doInBackground(URL... urls) {

                grafanaView.getSettings().setJavaScriptEnabled(true);
                String grafanaIP = getIntent().getStringExtra("GRAFANA_IP");
                grafanaView.loadUrl("http://"+grafanaIP+":3000");
                return new Long(1);
        }
    }
}



