#!/usr/bin/python3

########################################################################
# Copyright (c) 2018-2019 Robert Bosch GmbH
#
# This program and the accompanying materials are made
# available under the terms of the Eclipse Public License 2.0
# which is available at https://www.eclipse.org/legal/epl-2.0/
#
# SPDX-License-Identifier: EPL-2.0
########################################################################


# This provider gets data from gpsd (http://www.catb.org/gpsd/)

from gps3 import gps3
import sys
import threading

class Provider():
    def __init__(self, config):
        print("Init gpsd provider...")
        if "Provider.gpsd" not in config:
            print("Provider.gpsd section missing from configuration, exiting")
            sys.exit(-1)
        
        provider_config=config['Provider.gpsd']
        gpsd_host=provider_config.get('host','127.0.0.1')
        gpsd_port=provider_config.get('port','2947')

        print("Trying to connect gpsd at "+str(gpsd_host)+" port "+str(gpsd_port))
        self.connect(gpsd_host,gpsd_port)

        self.position = { "valid":False, "lat":"0", "lon":"0", "alt":"0", "hdop":"0", "speed":"0" }
        self.running = True
        self.lock = threading.Lock()

    def loop(self, gpsd_socket):
        print("gpsd receive loop started")
        data_stream = gps3.DataStream()
        gpsd_socket.watch()
        for new_data in gpsd_socket:
            if new_data:
                if self.running == False:
                    return
                data_stream.unpack(new_data)
                print("GPSD data....")

                self.lock.acquire()
                self.position['lat']=data_stream.TPV['lat']
                self.position['lon']=data_stream.TPV['lon']            
                self.position['alt']=data_stream.TPV['alt']
                self.position['speed']=data_stream.TPV['speed']
                if "hdop" in data_stream.SKY:
                    self.position['hdop']=data_stream.SKY['hdop']

                #check if self.position valid
                if 'n/a' in self.position.values():
                    self.position['valid']=False
                else:
                    self.position['valid']=True
                self.lock.release()


    def getPosition(self):
        self.lock.acquire()
        p=self.position
        self.lock.release()
        return p


    def connect(self,host,port):
        gpsd_socket = gps3.GPSDSocket()
        gpsd_socket.connect(host, port)  #TODO: Find out when it fails

    #    loop(gpsd_socket)
        t = threading.Thread(target=self.loop, args=(gpsd_socket,))
        t.start()
        


    def shutdown(self):
        self.running=False


