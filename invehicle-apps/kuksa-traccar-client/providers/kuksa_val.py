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


import os, sys, json
import csv
import time
import threading
from providers.kuksa.clientComm import VSSClientComm

class Provider():

    # Constructor
    def __init__(self, config):
        print("Init kuksa_val provider...")
        if "Provider.kuksa_val" not in config:
            print("Provider.kuksa_val section missing from configuration, exiting")
            sys.exit(-1)
        provider_config=config['Provider.kuksa_val']
        self.client = VSSClientComm(provider_config)
        self.client.start()
        self.token = provider_config.get('token', "token.json")
        self.client.authorize(self.token)

    def shutdown(self):
        self.client.stopComm()

    def getValue(self, data, path):
        print("find " + path)
        if path not in data:
            print(path + " not found!")
            return 0
        if isinstance(data[path], float) or data[path].isnumeric():
            return data[path]
        return 0

    def getPosition(self):
        resp = self.client.getValue("Vehicle.Cabin.Infotainment.Navigation.CurrentLocation")
    
        position = { "valid": False }
        print("resp: " + str(resp))
        resp = json.loads(resp)
        print("resp: " + str(resp))

        try:
            data = resp['value']
            position = { "valid":True, 
                "alt": self.getValue(data, 'Vehicle.Cabin.Infotainment.Navigation.CurrentLocation.Altitude'),
                "lat": self.getValue(data, 'Vehicle.Cabin.Infotainment.Navigation.CurrentLocation.Latitude'),
                "lon": self.getValue(data, 'Vehicle.Cabin.Infotainment.Navigation.CurrentLocation.Longitude'),
                "hdop": self.getValue(data, 'Vehicle.Cabin.Infotainment.Navigation.CurrentLocation.Accuracy'),
                "speed": self.getValue(data, 'Vehicle.Cabin.Infotainment.Navigation.CurrentLocation.Speed')}
        except TypeError as e:
            print("type error: " + str(e))
            pass
        except KeyError as e:
            print("key error: " + str(e))
            pass
        except :
            print("Unexpected error:", sys.exc_info())
            
        
        return position

