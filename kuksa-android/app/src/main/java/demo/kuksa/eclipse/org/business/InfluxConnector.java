package demo.kuksa.eclipse.org.business;

import android.content.SharedPreferences;
import android.util.Log;

import org.json.JSONArray;
import org.json.JSONObject;

import java.io.BufferedInputStream;
import java.io.BufferedReader;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.net.HttpURLConnection;
import java.net.URL;
import java.util.HashMap;

import android.preference.PreferenceManager;

import static android.content.Context.MODE_PRIVATE;

public class InfluxConnector {


    private String influxURLRPM;
    private String influxURLSpeed;
    private String influxURLError;
    private String influxURLFuel;
    private String influxURLGPS;

    public InfluxConnector(String influxIP) {
        influxURLRPM = "http://"+ influxIP +":8086/query?db=devices&u=admin&p=admin&q=SELECT%20Last(%22RPM%22)%20FROM%20%22$$$%22%20WHERE%20time%20%3E%20now()%20-%202s";
        influxURLSpeed = "http://"+ influxIP +":8086/query?db=devices&u=admin&p=admin&q=SELECT%20Last(%22SPEED%22)%20FROM%20%22$$$%22%20WHERE%20time%20%3E%20now()%20-%202s";
        influxURLError = "http://"+ influxIP +":8086/query?db=devices&u=admin&p=admin&q=SELECT%20Last(%22ERROR1%22)%20FROM%20%22$$$%22%20";
        influxURLFuel = "http://"+influxIP+":8086/query?db=devices&u=admin&p=admin&q=SELECT%20Last(%22FUEL%22)%20FROM%20%22$$$%22%20";
        influxURLGPS = "http://"+influxIP+":8086/query?db=devices&u=admin&p=admin&q=SELECT%20Last(%22GPS%22)%20FROM%20%22$$$%22%20WHERE%20time%20%3E%20now()%20-%202s";
    }

    public Double[] getGPSData ( String vehicleID ) {
        HttpURLConnection urlConnection = null;
        String gpsURL = influxURLGPS.replace("$$$",vehicleID);
        try {
            URL url = new URL(gpsURL);
            urlConnection = (HttpURLConnection) url.openConnection();
            InputStream in = new BufferedInputStream(urlConnection.getInputStream());

            // convert to string.
            BufferedReader bReader = new BufferedReader(new InputStreamReader(in));
            String line = null;
            JSONArray jarr = null;
            JSONObject jObject = null;

            line = bReader.readLine();
            jObject = new JSONObject(line);
            if(jObject != null && jObject.has("results")) {
                jarr = jObject.getJSONArray("results");
            }

            Log.i("kuksa_dev","GPS Raw line from Http Call= "+line);


            if(jarr != null) {
                JSONObject results = jarr.getJSONObject(0);
                JSONArray series = results.getJSONArray("series");
                JSONObject valuesObj = series.getJSONObject(0);
                JSONArray values = valuesObj.getJSONArray("values").getJSONArray(0);
                String rawResponse = values.toString().replace("\"", "");
                rawResponse = rawResponse.replace("]", "");
                String gpsValuesString[] = rawResponse.split(",");
                Log.d("GPS Values = " , gpsValuesString[1]+", " + gpsValuesString[2]);

                Double[] gpsValues = new Double[2];
                gpsValues[0] = Double.parseDouble(gpsValuesString[1]);
                gpsValues[1] = Double.parseDouble(gpsValuesString[2]);
                Log.i("Kuksa-dev", "GPS Value extracted from HTTP call = " + gpsValues[0] + " " + gpsValues[1]);
                return gpsValues;
            } else {
                return null;
            }

        }catch(Exception e){
            Log.e("Exception while collecting GPS data from influxdb", e.toString());
        } finally {
            if(urlConnection != null)
                urlConnection.disconnect();
        }
        return null;

    }

    public String getVehicleRPM ( String vehicleID ) {
        HttpURLConnection urlConnection = null;
        String rpmURL = influxURLRPM.replace("$$$",vehicleID);
        try {
        URL url = new URL(rpmURL);
        urlConnection = (HttpURLConnection) url.openConnection();
        InputStream in = new BufferedInputStream(urlConnection.getInputStream());

        // convert to string.
            BufferedReader bReader = new BufferedReader(new InputStreamReader(in));
            String line = null;
            JSONArray jarr = null;
            JSONObject jObject = null;

            line = bReader.readLine();
            jObject = new JSONObject(line);
            if(jObject != null && jObject.has("results")) {
                jarr = jObject.getJSONArray("results");
            }

            Log.i("kuksa_dev","RPM Raw line from Http Call= "+line);


        if(jarr != null) {
            JSONObject results = jarr.getJSONObject(0);
            JSONArray series = results.getJSONArray("series");
            JSONObject valuesObj = series.getJSONObject(0);
            JSONArray values = valuesObj.getJSONArray("values").getJSONArray(0);
            String value = values.getString(1);
            Log.i("Kuksa-dev", "RPM Value extracted from HTTP call = " + value);
            return value;
        } else {
            return null;
        }

        }catch(Exception e){
            Log.e("Kuksa-dev", e.toString());
        } finally {
            if(urlConnection != null)
               urlConnection.disconnect();
        }
        return null;

    }

    public String getVehicleSpeed ( String vehicleID ) {
        HttpURLConnection urlConnection = null;
        String speedURL = influxURLSpeed.replace("$$$",vehicleID);
        try {
            URL url = new URL(speedURL);
            urlConnection = (HttpURLConnection) url.openConnection();
            InputStream in = new BufferedInputStream(urlConnection.getInputStream());

            // convert to string.
            BufferedReader bReader = new BufferedReader(new InputStreamReader(in));
            String line = null;
            JSONArray jarr = null;
            JSONObject jObject = null;

            line = bReader.readLine();
            jObject = new JSONObject(line);
            if(jObject != null && jObject.has("results")) {
                jarr = jObject.getJSONArray("results");
            }

            Log.i("kuksa_dev","Speed Raw line from Http Call= "+line);


            if(jarr != null) {
                JSONObject results = jarr.getJSONObject(0);
                JSONArray series = results.getJSONArray("series");
                JSONObject valuesObj = series.getJSONObject(0);
                JSONArray values = valuesObj.getJSONArray("values").getJSONArray(0);
                String value = values.getString(1);
                Log.i("Kuksa-dev", "Speed Value extracted from HTTP call = " + value);
                return value;
            } else {
                return null;
            }

        }catch(Exception e){
            Log.e("Kuksa-dev", e.toString());
        } finally {
            if(urlConnection != null)
                urlConnection.disconnect();
        }
        return null;

    }

    public String getFuelStatus ( String vehicleID ) {
        HttpURLConnection urlConnection = null;
        String fuelURL = influxURLFuel.replace("$$$",vehicleID);
        try {
            URL url = new URL(fuelURL);
            urlConnection = (HttpURLConnection) url.openConnection();
            InputStream in = new BufferedInputStream(urlConnection.getInputStream());

            // convert to string.
            BufferedReader bReader = new BufferedReader(new InputStreamReader(in));
            String line = null;
            JSONArray jarr = null;
            JSONObject jObject = null;

            line = bReader.readLine();
            jObject = new JSONObject(line);
            if(jObject != null && jObject.has("results")) {
                jarr = jObject.getJSONArray("results");
            }

            Log.i("kuksa_dev","Fuel Raw line from Http Call= "+line);


            if(jarr != null) {
                JSONObject results = jarr.getJSONObject(0);
                JSONArray series = results.getJSONArray("series");
                JSONObject valuesObj = series.getJSONObject(0);
                JSONArray values = valuesObj.getJSONArray("values").getJSONArray(0);
                String value = values.getString(1);
                Log.i("Kuksa-dev", "Fuel Value extracted from HTTP call = " + value);
                return value;
            } else {
                return null;
            }

        }catch(Exception e){
            Log.e("Kuksa-dev", e.toString());
        } finally {
            if(urlConnection != null)
                urlConnection.disconnect();
        }
        return null;

    }

    public boolean checkDTC ( String vehicleID ) {

        HttpURLConnection urlConnection = null;
        String dtcURL = influxURLError.replace("$$$",vehicleID);
        try {
            URL url = new URL(dtcURL);
            urlConnection = (HttpURLConnection) url.openConnection();
            InputStream in = new BufferedInputStream(urlConnection.getInputStream());

            // convert to string.
            BufferedReader bReader = new BufferedReader(new InputStreamReader(in));
            String line = null;
            JSONArray jarr = null;
            JSONObject jObject = null;

            line = bReader.readLine();
            jObject = new JSONObject(line);
            if(jObject != null && jObject.has("results")) {
                jarr = jObject.getJSONArray("results");
            }

            Log.i("kuksa_dev","Speed Raw line from Http Call= "+line);


            if(jarr != null) {
                JSONObject results = jarr.getJSONObject(0);
                JSONArray series = results.getJSONArray("series");
                JSONObject valuesObj = series.getJSONObject(0);
                JSONArray values = valuesObj.getJSONArray("values").getJSONArray(0);
                String value = values.getString(1);
                Log.i("Kuksa-dev", "Speed Value extracted from HTTP call = " + value);
                if("1".equals(value)){
                    return true;
                } else {
                    return false;
                }

            } else {
                return false;
            }

        }catch(Exception e){
            Log.e("Kuksa-dev", e.toString());
        } finally {
            if(urlConnection != null)
                urlConnection.disconnect();
        }
        return false;

    }
}
