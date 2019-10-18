package demo.kuksa.eclipse.org.devdemo;

import android.app.Activity;
import android.graphics.Color;
import android.os.AsyncTask;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.View;
import android.widget.ImageView;
import android.widget.ProgressBar;
import android.widget.RelativeLayout;
import android.widget.TextView;


import java.net.MalformedURLException;
import java.net.URL;

import demo.kuksa.eclipse.org.business.InfluxConnector;

public class MycarActivity extends AppCompatActivity {


    TextView rpmMeter, speedometer;
    ProgressBar fuelMeter;
    ImageView checkEngine;

    private volatile boolean runBG;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        setContentView(R.layout.activity_mycar);

        RelativeLayout rl = findViewById(R.id.mycarLayout);
        rl.setBackgroundColor(Color.BLACK);
        speedometer = findViewById(R.id.speedView);
        speedometer.setText("0");
        speedometer.setEnabled(false);

        rpmMeter = findViewById(R.id.rpmView);
        rpmMeter.setText("0");
        rpmMeter.setEnabled(false);

        fuelMeter = findViewById(R.id.fuelView);
        fuelMeter.setProgress(0);
        fuelMeter.setMin(0);
        fuelMeter.setMax(100);

        checkEngine = findViewById(R.id.checkEngine);
        checkEngine.setVisibility(View.INVISIBLE);
    }

    @Override
    protected void onStart() {
        super.onStart();
        runBG = true;
        try {
            new GrafTask().execute(new URL("https://developer.mozilla.org"));
        } catch (MalformedURLException e) {
            Log.e("kuksa-dev","URL not ok!");
        }
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        runBG = false;
    }

    @Override
    protected void onPause() {
        super.onPause();
        runBG = false;
    }

    @Override
    protected void onResume() {
        super.onResume();
    }

    private class GrafTask extends AsyncTask<URL, Integer, Long> {


        @Override
        protected Long doInBackground(URL... urls) {

            while (runBG) {
                InfluxConnector infconn = new InfluxConnector(getIntent().getStringExtra("INFLUXDB_IP"));
                String vehicleID = getIntent().getStringExtra("VEHICLE_ID");
                 final String rpm = infconn.getVehicleRPM(vehicleID);
                 final String speed = infconn.getVehicleSpeed(vehicleID);
                 int count = 3001;

                 if( count > 3000) {
                      final String fuel = infconn.getFuelStatus(vehicleID);
                      runOnUiThread(new Runnable() {
                          @Override
                          public void run() {
                              if (fuel != null) {
                                  fuelMeter.setProgress(Integer.valueOf(fuel));
                              }
                          }
                      });
                      count = 0;
                 }
                 final boolean engineMal = infconn.checkDTC(vehicleID);
                runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        if (rpm != null) {
                            rpmMeter.setText(rpm);
                        } else {
                            rpmMeter.setText("0");
                        }
                        if (speed != null) {
                            speedometer.setText(speed);
                        } else {
                            speedometer.setText("0");
                        }
                        if (engineMal) {
                            checkEngine.setVisibility(View.VISIBLE);
                        } else {
                            checkEngine.setVisibility(View.INVISIBLE);
                        }
                    }
                });

                try {
                    Thread.sleep(1);
                    count++;
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }
            return new Long(1);
        }
    }
}
