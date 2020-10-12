#! /usr/bin/env python

########################################################################
# Copyright (c) 2020 Robert Bosch GmbH
#
# This program and the accompanying materials are made
# available under the terms of the Eclipse Public License 2.0
# which is available at https://www.eclipse.org/legal/epl-2.0/
#
# SPDX-License-Identifier: EPL-2.0
########################################################################
# This provider gets data from a simple log file which contains lines
# formatted 
# lat,lon
# All other values will be reported as 0


from gps3 import gps3
import threading
import configparser
import os, sys, json, signal
import csv
import time
import threading, queue, ssl
import asyncio, websockets, pathlib
from clientComm import VSSClientComm

class Kuksa_Client():

    # Constructor
    def __init__(self, config):
        print("Init kuksa_val provider...")
        if "Provider.kuksa_val" not in config:
            print("Provider.kuksa_val section missing from configuration, exiting")
            sys.exit(-1)
        provider_config=config['Provider.kuksa_val']
        self.sendMsgQueue = queue.Queue()
        self.recvMsgQueue = queue.Queue()
        self.client = VSSClientComm(self.sendMsgQueue, self.recvMsgQueue, provider_config)
        self.client.start()
        self.token = provider_config.get('token', "token.json")
        self.authorize()
        
    def authorize(self):
        if os.path.isfile(self.token):
            with open(self.token, "r") as f:
                self.token = f.readline()

        req = {}
        req["requestId"] = 1238
        req["action"]= "authorize"
        req["tokens"] = self.token
        jsonDump = json.dumps(req)
        self.sendMsgQueue.put(jsonDump)
        print(req)
        resp = self.recvMsgQueue.get(timeout = 1)
        print(resp)

    def shutdown(self):
        self.client.stopComm()

    def setValue(self, path, value):
        if 'n/a' == value:
            print(path + "has an invalid value")
            return
        req = {}
        req["requestId"] = 1235
        req["action"]= "set"
        req["path"] = path
        req["value"] = value
        jsonDump = json.dumps(req)
        print(jsonDump)
        self.sendMsgQueue.put(jsonDump)
        resp = self.recvMsgQueue.get(timeout = 1)
        print(resp)
        
    def setPosition(self, position):
        self.setValue('Vehicle.Cabin.Infotainment.Navigation.CurrentLocation.Altitude', position['alt'])
        self.setValue('Vehicle.Cabin.Infotainment.Navigation.CurrentLocation.Latitude', position["lat"])
        self.setValue('Vehicle.Cabin.Infotainment.Navigation.CurrentLocation.Longitude', position["lon"])
        self.setValue('Vehicle.Cabin.Infotainment.Navigation.CurrentLocation.Accuracy', position["hdop"])
        self.setValue('Vehicle.Cabin.Infotainment.Navigation.CurrentLocation.Speed', position["speed"])

class GPSD_Client():
    def __init__(self, config, consumer):
        print("Init gpsd provider...")
        if "Provider.gpsd" not in config:
            print("Provider.gpsd section missing from configuration, exiting")
            sys.exit(-1)
        
        self.consumer = consumer
        provider_config=config['Provider.gpsd']
        gpsd_host=provider_config.get('host','127.0.0.1')
        gpsd_port=provider_config.get('port','2947')

        print("Trying to connect gpsd at "+str(gpsd_host)+" port "+str(gpsd_port))


        gpsd_socket = gps3.GPSDSocket()
        gpsd_socket.connect(gpsd_host, gpsd_port)  #TODO: Find out when it fails
        self.position = {"lat":"0", "lon":"0", "alt":"0", "hdop":"0", "speed":"0" }
        self.running = True

        self.thread = threading.Thread(target=self.loop, args=(gpsd_socket,))
        self.thread.start()

    def loop(self, gpsd_socket):
        print("gpsd receive loop started")
        data_stream = gps3.DataStream()
        gpsd_socket.watch()
        for new_data in gpsd_socket:
            if new_data:
                if self.running == False:
                    return
                data_stream.unpack(new_data)

                self.position['lat']=data_stream.TPV['lat']
                self.position['lon']=data_stream.TPV['lon']            
                self.position['alt']=data_stream.TPV['alt']
                self.position['speed']=data_stream.TPV['speed']
                if "hdop" in data_stream.SKY:
                    self.position['hdop']=data_stream.SKY['hdop']

                self.consumer.setPosition(self.position)
                time.sleep(0.1)


    def shutdown(self):
        self.running=False
        self.consumer.shutdown()
        self.thread.join()

            
        
if __name__ == "__main__":
    config_candidates=['traccar-client.ini', '/etc/traccar-client.ini']
    for candidate in config_candidates:
        if os.path.isfile(candidate):
            configfile=candidate
            break
    if configfile is None:
        print("No configuration file found. Exiting")
        sys.exit(-1)
    config = configparser.ConfigParser()
    config.read(configfile)
    
    client = GPSD_Client(config, Kuksa_Client(config))

    def terminationSignalreceived(signalNumber, frame):
        print("Received termination signal. Shutting down")
        client.shutdown()
    signal.signal(signal.SIGINT, terminationSignalreceived)
    signal.signal(signal.SIGQUIT, terminationSignalreceived)
    signal.signal(signal.SIGTERM, terminationSignalreceived)

