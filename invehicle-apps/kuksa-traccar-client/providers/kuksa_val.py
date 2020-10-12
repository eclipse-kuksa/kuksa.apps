#! /usr/bin/python3

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


import os, sys, json
import csv
import time
import threading, queue, ssl
import asyncio, websockets, pathlib
from providers.kuksa.clientComm import VSSClientComm

class Provider():

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
        resp = self.recvMsgQueue.get(timeout = 1)
        print(resp)

    def shutdown(self):
        self.client.stopComm()

    def getPosition(self):
        req = {}
        req["requestId"] = 1234
        req["action"]= "get"
        req["path"] = "Vehicle.Cabin.Infotainment.Navigation.CurrentLocation"
        jsonDump = json.dumps(req)
        print(jsonDump)
        self.sendMsgQueue.put(jsonDump)
        resp = self.recvMsgQueue.get(timeout = 1)
    
        position = { "valid": False }
        resp = json.loads(resp)
        print(resp)

        try:
            data = resp['value']
            position = { "valid":True, 
                "alt": data['Vehicle.Cabin.Infotainment.Navigation.CurrentLocation.Altitude'],
                "lat": data['Vehicle.Cabin.Infotainment.Navigation.CurrentLocation.Latitude'],
                "lon": data['Vehicle.Cabin.Infotainment.Navigation.CurrentLocation.Longitude'],
                "hdop": 0, #data['Vehicle.Cabin.Infotainment.Navigation.CurrentLocation.Accuracy'],
                "speed": data['Vehicle.Cabin.Infotainment.Navigation.CurrentLocation.Speed']}
        except TypeError as e:
            print("type error: " + str(e))
            pass
        except KeyError as e:
            print("key error: " + str(e))
            pass
        except :
            print("Unexpected error:", sys.exc_info()[0])
            
        
        return position

