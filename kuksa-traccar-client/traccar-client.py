#!/usr/bin/python3

# Copyright (c) 2018 Eclipse KUKSA project
#
# This program and the accompanying materials are made available under the
# terms of the Eclipse Public License 2.0 which is available at
# http://www.eclipse.org/legal/epl-2.0
#
# SPDX-License-Identifier: EPL-2.0
#
# Contributors: Robert Bosch GmbH
#
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

config_candidates=['traccar-client.ini', '/etc/traccar-client.ini']



def instantiateProvider(provider, config):
    mod=importlib.import_module("providers."+provider)
    mod.initProvider(config)
    return mod
    

def main(args):
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
    traccar_publishing_interval = traccar_cfg.get('interval','30')

    if "Provider" not in config:
        print("Provider section missing from configuration, exiting")
        sys.exit(-1)

    provider_cfg = config['Traccar']
    location_provider = traccar_cfg.get('provider','gpsd')

    print("Will connect to "+str(traccar_server))
    print("Will publish location every "+str(traccar_publishing_interval))

    print("Location data will be provided by "+str(location_provider))

    print("Trying to intantiante "+str(location_provider)+" provider")
    provider=instantiateProvider(location_provider,config)

    while True:
        pos=provider.getPosition()
        print("Current pos "+str(pos))
        time.sleep(5)


if __name__ == "__main__":
    main(sys.argv)