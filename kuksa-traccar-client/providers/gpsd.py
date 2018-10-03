from gps3 import gps3
import sys
import threading


position = { "valid":False, "lat":"0", "lon":"0", "alt":"0", "hdop":"0", "speed":"0" }
t=None

def loop(gpsd_socket):
    global position
    print("gpsd receive loop started")
    data_stream = gps3.DataStream()
    gpsd_socket.watch()
    for new_data in gpsd_socket:
        if new_data:
            data_stream.unpack(new_data)
            print('Altitude = ', data_stream.TPV['alt'])
            print('Latitude = ', data_stream.TPV['lat'])

            position['lat']=data_stream.TPV['lat']
            position['lon']=data_stream.TPV['lon']            
            position['alt']=data_stream.TPV['alt']
            position['speed']=data_stream.TPV['speed']
            if "hdop" in data_stream.SKY:
                position['hdop']=data_stream.SKY['hdop']

            #check if position valid
            if 'n/a' in position.values():
                position['valid']=False
            else:
                position['valid']=True


def getPosition():
    global position
    print("Pos object is "+str(position))
    return position


def connect(host,port):
    gpsd_socket = gps3.GPSDSocket()
    gpsd_socket.connect(host, port)  #TODO: Find out when it fails

#    loop(gpsd_socket)
    t = threading.Thread(target=loop, args=(gpsd_socket,))
    t.start()
    
def initProvider(config):
    print("Init gpsd provider...")
    if "Provider.gpsd" not in config:
        print("Provider.gpsd section missing from configuration, exiting")
        sys.exit(-1)
    
    provider_config=config['Provider.gpsd']
    gpsd_host=provider_config.get('host','127.0.0.1')
    gpsd_port=provider_config.get('port','2947')

    print("Trying to connect gpsd at "+str(gpsd_host)+" port "+str(gpsd_port))
    connect(gpsd_host,gpsd_port)

    
