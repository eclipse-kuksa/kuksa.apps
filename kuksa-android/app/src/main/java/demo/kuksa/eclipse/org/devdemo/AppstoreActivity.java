package demo.kuksa.eclipse.org.devdemo;

import android.os.AsyncTask;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.webkit.WebView;

import java.net.URL;

public class AppstoreActivity extends AppCompatActivity {



    WebView appstoreView;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_appstore);
        //super.onCreate(savedInstanceState);


        appstoreView = (WebView) findViewById(R.id.appstoreView);
        appstoreView.getSettings().setLoadWithOverviewMode(true);
        appstoreView.getSettings().setUseWideViewPort(true);

        /*try {
            new GrafTask().execute(new URL("https://developer.mozilla.org"));
        } catch (MalformedURLException e) {
            Log.e("kuksa-dev","URL not ok!");
        }*/

        appstoreView.getSettings().setJavaScriptEnabled(true);
        String appStoreIP = getIntent().getStringExtra("APPSTORE_IP");

        appstoreView.loadUrl("http://"+"appstacle-hot-kuksa-appstore.westeurope.cloudapp.azure.com"+":8080");


    }

    private class GrafTask extends AsyncTask<URL, Integer, Long> {


        @Override
        protected Long doInBackground(URL... urls) {

            appstoreView.getSettings().setJavaScriptEnabled(true);
            String appStoreIP = getIntent().getStringExtra("APPSTORE_IP");
            appstoreView.loadUrl("http://"+appStoreIP+":8082");
            return new Long(1);
        }
    }
}
