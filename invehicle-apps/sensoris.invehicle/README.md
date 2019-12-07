## Sensoris Invehicle 

Sensoris is a vehicle sensor data specification, which allows data abstraction and compressed vehicle sensor data exchange with the cloud. For more [info](https://sensor-is.org/)

This is a demo application which connects to the sensoris cloud app (running remotely on the cloud) and uploads GPS data.
This app only serves as an example.

The GPS Data is taken from a running W3C-VIS Server over a websocket connection. The GPS data is then compressed using the protobuf files and then pushed to the sensoris cloud app every second.


## Build

Requires Boost > v1.67

Requires Protobuf = v3.6 



