# Traccar client

Simple KUKSA app that can send location data to the Traccar GPS tracking suite (https://www.traccar.org).

Does not use any KUKSA cloud service, instead it demonstrates how the platform can interact with existing cloud services

## Requirements
The client can access GPSD. For this it uses the gps3 library: https://pypi.org/project/gps3 or  https://github.com/wadda/gps3 
It uses python requests for talking to Traccar

Install it with 

`sudo pip3 install -r ./requirements.txt`

KUKSA-traccar-client needs a running traccar instance to connect to. Traccar installation instructions can be found here https://www.traccar.org/.
kuksa-traccar-client uses the OSMAND protocol, which usuall is available at port 5055 of a running traccar instance 

## Configuration file
The client tries to read traccar-client.ini in the current working directory, if that file is not found it tries /etc/traccar-client.ini

Here is the default configurationf or reference
```
[Traccar]
#Host and port where Traccar is running. Needs to start with http or (better) https and can include port number
server = http://127.0.0.1:5055
# Reporting frequency: Decides how often (every n seconds) data should be send to Traccar. Note: This is independent from the speed of the providers. Always the last known position is send. Only if a provider can report, that no valid position is available, the message will be supressed
interval = 1
# an identifer identifies a device in the Traccar server
identifer = 12345678



[Provider]
#Select which provider is used to acquire GPS data. Currently only gpsd and simplelog is supported
provider = simplelog

# Provider specific settings. 

[Provider.gpsd]
host = 127.0.0.1
port = 2947

[Provider.simplelog]
#simplelog expects a simple CSV containing only lat/long positions and no timestamp
file = simplelog_example.csv
#simplelog assumes all logged positions are <interval> seconds apart. No interpolation is done, the position will just jump
interval = 1
```



## Testing
In default configuration kuksa-traccar-client plays GPS data from an included logfile. This is usually enough for testing. 

here is a way how to get "real" GPS data easily on a PC with no GPS module.

Install gpsd on your Linux machine. If your distro starts it automatically stop it. In Ubuntu like this:

`sudo systemctl stop gpsd`
`sudo systemctl stop gpsd.socket`


Then start gpsd in Debug mode

`sudo gpsd -G  -n -N -D4 udp://*:3333`


Now GPSd awaits GPS data in NMEA format via UDP on port 3333. Many Android Apps can provide it, a totally free one is GPSDClient

https://play.google.com/store/apps/details?id=io.github.tiagoshibata.gpsdclient&rdid=io.github.tiagoshibata.gpsdclient

Start it, and let is stream to the ip of the computer you run GPSD on port 3333.

_Caveat_: It will only work if your phone has a _real_ GPS fix. (Just becasue Google maps shows some location guestimated form WiFi/cell towers is not enough)

## Build docker image

Use the instructions available in the docker folder to build the docker image.

Use the kuksa-publisher to publish the app in the kuksa-appstore.
