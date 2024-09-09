package demo.kuksa.eclipse.org.devdemo;

import android.os.AsyncTask;
import android.support.v4.app.FragmentActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;

import com.google.android.gms.maps.CameraUpdateFactory;
import com.google.android.gms.maps.GoogleMap;
import com.google.android.gms.maps.OnMapReadyCallback;
import com.google.android.gms.maps.SupportMapFragment;
import com.google.android.gms.maps.model.LatLng;
import com.google.android.gms.maps.model.MarkerOptions;

import java.net.MalformedURLException;
import java.net.URL;

import demo.kuksa.eclipse.org.business.InfluxConnector;

public class MapsActivity extends FragmentActivity implements OnMapReadyCallback {

    private GoogleMap mMap;
    private volatile boolean runBG;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_maps);
        // Obtain the SupportMapFragment and get notified when the map is ready to be used.
        SupportMapFragment mapFragment = (SupportMapFragment) getSupportFragmentManager()
                .findFragmentById(R.id.map);
        mapFragment.getMapAsync(this);
    }


    /**
     * Manipulates the map once available.
     * This callback is triggered when the map is ready to be used.
     * This is where we can add markers or lines, add listeners or move the camera. In this case,
     * we just add a marker near Sydney, Australia.
     * If Google Play services is not installed on the device, the user will be prompted to install
     * it inside the SupportMapFragment. This method will only be triggered once the user has
     * installed Google Play services and returned to the app.
     */
    @Override
    public void onMapReady(GoogleMap googleMap) {
        mMap = googleMap;

        // Add a marker in Sydney and move the camera
        LatLng sydney = new LatLng(-34, 151);
        mMap.addMarker(new MarkerOptions().position(sydney).title("Marker in Sydney"));
        mMap.moveCamera(CameraUpdateFactory.newLatLng(sydney));
        mMap.setMinZoomPreference(15.0f);
        mMap.setMaxZoomPreference(30.0f);


    }


    @Override
    protected void onStart() {
        super.onStart();
        runBG = true;
        try {
            new MapsActivity.mapTask().execute(new URL("https://developer.mozilla.org"));
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


    private class mapTask extends AsyncTask<URL, Integer, Long> {


        @Override
        protected Long doInBackground(URL... urls) {

            while (runBG) {
                InfluxConnector infconn = new InfluxConnector(getIntent().getStringExtra("INFLUXDB_IP"));
                String vehicleID = getIntent().getStringExtra("VEHICLE_ID");
                final Double[] gpsData = infconn.getGPSData(vehicleID);
                if (gpsData != null) {
                    runOnUiThread(new Runnable() {
                        @Override
                        public void run() {
                            LatLng newPoint = new LatLng(gpsData[0], gpsData[1]);
                            mMap.addMarker(new MarkerOptions().position(newPoint).title("Latest"));
                            mMap.moveCamera(CameraUpdateFactory.newLatLng(newPoint));

                        }
                    });
                }
                try {
                    Thread.sleep(1000);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }
            return new Long(1);
        }

    }

}
