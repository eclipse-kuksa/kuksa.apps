package demo.kuksa.eclipse.org.devdemo;

import android.content.ActivityNotFoundException;
import android.content.ComponentName;
import android.content.Intent;
import android.content.SharedPreferences;
import android.net.Uri;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.preference.PreferenceManager;
import android.support.v7.widget.Toolbar;
import android.util.Log;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.view.View;
import android.widget.ImageButton;

import java.util.HashMap;


public class MainActivity extends AppCompatActivity implements View.OnClickListener, SharedPreferences.OnSharedPreferenceChangeListener{
    HashMap<String,String> appIps = new HashMap<>();
    ImageButton myCar_bt, appStore_bt, cloud_bt, fixCar_bt, maps_bt;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        Toolbar toolbar = findViewById(R.id.toolbar);
        setSupportActionBar(toolbar);

        myCar_bt = findViewById(R.id.mycar_button);
        fixCar_bt = findViewById(R.id.fixcar_button);
        cloud_bt = findViewById(R.id.cloud_button);
        maps_bt = findViewById(R.id.track_button);
        appStore_bt = findViewById(R.id.appstore_button);

        myCar_bt.setOnClickListener(this);
        appStore_bt.setOnClickListener(this);
        cloud_bt.setOnClickListener(this);
        maps_bt.setOnClickListener(this);
        fixCar_bt.setOnClickListener(this);

        setupSharedPreferences();
        Log.d ("kuksa-dev", " on create method called.");
    }

    private void setupSharedPreferences() {
        SharedPreferences sharedPreferences = PreferenceManager.getDefaultSharedPreferences(this);
        sharedPreferences.registerOnSharedPreferenceChangeListener(this);
    }

    @Override
    public void onSharedPreferenceChanged(SharedPreferences sharedPreferences, String key) {

       /* if (key.equals("display_text")) {
            setTextVisible(sharedPreferences.getBoolean("display_text",true));
        }*/
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        MenuInflater menuInflater = getMenuInflater();
        menuInflater.inflate(R.menu.menu_main, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        int id = item.getItemId();
        if (id == R.id.action_settings) {
            Intent intent = new Intent(MainActivity.this, SettingsActivity.class);
            startActivity(intent);
            return true;
        }
        return super.onOptionsItemSelected(item);
    }

    @Override
    protected void onStart() {
        super.onStart();

        // retrieve new preferences.
        SharedPreferences prefs = PreferenceManager.getDefaultSharedPreferences(this);
        // then you use
        appIps.put("InfluxIP", prefs.getString("addressIF","localhost"));
        appIps.put("VehicleID", prefs.getString("vehicleID","4711"));
        appIps.put("DispatchIP", prefs.getString("addressDisp","localhost"));
        appIps.put("AppstoreIP", prefs.getString("addressAPPS","localhost"));
        appIps.put("TraccarIP", prefs.getString("addressTrac","localhost"));

        Log.d ("kuksa-dev", " on start method ");
    }

    @Override
    protected void onRestart() {
        super.onRestart();
        Log.d ("kuksa-dev", " on restart method ");
    }

    @Override
    protected void onResume() {
        super.onResume();
        Log.d ("kuksa-dev", " on resume method ");
    }

    @Override
    protected void onPause() {
        super.onPause();
        Log.d ("kuksa-dev", " on pause method ");
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        PreferenceManager.getDefaultSharedPreferences(this)
                .unregisterOnSharedPreferenceChangeListener(this);
        Log.d ("kuksa-dev", " on destroy method ");
    }



    @Override
    public void onClick(View view) {
        switch (view.getId()) {

            case R.id.appstore_button:
                Intent appStoreIntent = new Intent(this, AppstoreActivity.class);
                appStoreIntent.putExtra("APPSTORE_IP", appIps.get("AppstoreIP"));
                startActivity(appStoreIntent);
                break;

            case R.id.cloud_button:
                Intent cloudIntent = new Intent(this, CloudActivity.class);
                cloudIntent.putExtra("GRAFANA_IP", appIps.get("GrafanaIP"));
                startActivity(cloudIntent);
                break;

            case R.id.track_button:
                Intent mapsIntent = new Intent(this, MapsActivity.class);
                mapsIntent.putExtra("INFLUXDB_IP",appIps.get("InfluxIP"));
                mapsIntent.putExtra("VEHICLE_ID",appIps.get("VehicleID"));
                startActivity(mapsIntent);

                /*String traccarIP = appIps.get("TraccarIP");
                String traccar_url = "http://"+traccarIP;
                try {
                    Intent in = new Intent("android.intent.action.MAIN");
                    in.setComponent(ComponentName.unflattenFromString("com.android.chrome/com.android.chrome.Main"));
                    in.addCategory("android.intent.category.LAUNCHER");
                    in.setData(Uri.parse(traccar_url));
                    startActivity(in);
                }
                catch(ActivityNotFoundException e) {
                    // Chrome is not installed
                    Intent in = new Intent(Intent.ACTION_VIEW, Uri.parse(traccar_url));
                    startActivity(in);
                }*/

                break;

            case R.id.mycar_button:
                Intent mycarIntent = new Intent(this, MycarActivity.class);
                mycarIntent.putExtra("INFLUXDB_IP",appIps.get("InfluxIP"));
                mycarIntent.putExtra("VEHICLE_ID",appIps.get("VehicleID"));
                startActivity(mycarIntent);
                break;

            case R.id.fixcar_button:
                Intent fixMycarIntent = new Intent(this, FixMyCarActivity.class);
                startActivity(fixMycarIntent);
                break;
        }
    }
}
