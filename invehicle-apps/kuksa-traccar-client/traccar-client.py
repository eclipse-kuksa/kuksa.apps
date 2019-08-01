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


# This program can act as device for the Traccar suite. It 
# gathers postion information form GPSd or from a logfile
# It currently uses OSMAND "protocol" to talk to Traccar, 
# see here: https://www.traccar.org/osmand/


import sys
import configparser
import importlib
import time
from providers import *
from os import path
from datetime import datetime
import requests
import signal

config_candidates=['traccar-client.ini', '/etc/traccar-client.ini']


RUNNING=True

def terminationSignalreceived(signalNumber, frame):
	global RUNNING
	print("Received termination signal. Shutting down")
	RUNNING=False
	PROVIDER.shutdown()


def instantiateProvider(provider, config):
    mod=importlib.import_module("providers."+provider)
    mod.initProvider(config)
    return mod
    

def main(args):
    global RUNNING, PROVIDER
    config = configparser.ConfigParser()

    configfile=None

    for candidate in config_candidates:
        if path.isfile(candidate):
            configfile=candidate
            break
    if configfile is None:
        print("No configuration file found. Exiting")
        sys.exit(-1)


    print("Reading configuration from "+str(configfile))
    config.read(configfile)

    if "Traccar" not in config:
        print("Traccar section missing from configuration, exiting")
        sys.exit(-1)

    traccar_cfg = config['Traccar']
    traccar_server              = traccar_cfg.get('server','http://localhost:8080')
    traccar_identifier = traccar_cfg.get('identifer','000000')
    traccar_publishing_interval = traccar_cfg.getint('interval',30)

    if "Provider" not in config:
        print("Provider section missing from configuration, exiting")
        sys.exit(-1)

    provider_cfg = config['Provider']
    location_provider = provider_cfg.get('provider','gpsd')

    print("Will connect to "+str(traccar_server))
    print("Will publish location every "+str(traccar_publishing_interval))
    print("My id is  "+str(traccar_identifier))
    

    print("Location data will be provided by "+str(location_provider))

    print("Trying to instantiate "+str(location_provider)+" provider")
    PROVIDER=instantiateProvider(location_provider,config)

    while RUNNING==True:
        pos=PROVIDER.getPosition()
        print("Current pos "+str(pos))
        if pos['valid']:
            request="id={}&lat={}&lon={}&timestamp={}&hdop={}&altitude={}&speed={}".format(traccar_identifier,pos['lat'],pos['lon'],datetime.now(),pos['hdop'],pos['alt'],pos['speed'])

            url=traccar_server+"/?"+request
            print("Will GET "+str(url))
            try:
                r = requests.get(url, timeout=10)
                if r.status_code != 200:
                    print("Error from Traccar server. HTTP status "+str(r.status_code))
            except requests.exceptions.RequestException as e:
                    print("Error contacting Traccar server:"+str(e))

            
        else:
            print("No valid position. Not posting")


            #post to Traccar
        time.sleep(traccar_publishing_interval)


if __name__ == "__main__":
    signal.signal(signal.SIGINT, terminationSignalreceived)
    signal.signal(signal.SIGQUIT, terminationSignalreceived)
    signal.signal(signal.SIGTERM, terminationSignalreceived)
    main(sys.argv)
