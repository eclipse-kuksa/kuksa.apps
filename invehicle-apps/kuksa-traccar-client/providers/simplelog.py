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

# This provider gets data from a simple log file which contains lines
# formatted 
# lat,lon
# All other values will be reported as 0


import csv
import time
import threading

class Provider():
    def __init__(self, config):

        print("Init simplelog provider...")
        if "Provider.simplelog" not in config:
            print("Provider.simplelog section missing from configuration, exiting")
            sys.exit(-1)
        
        provider_config=config['Provider.simplelog']
        simplelog_file=provider_config.get('file','log.csv')
        self.simplelog_interval=provider_config.getint('interval',1)

        print("Trying to read simeplelog from "+str(simplelog_file)+" with a position every  "+str(self.simplelog_interval)+" s")

        csv_f=open(simplelog_file)
        csv_reader=csv.reader(csv_f, delimiter=',')
        
        self.position = { "valid":False, "lat":"0", "lon":"0", "alt":"0", "hdop":"0", "speed":"0" }

        self.lock=threading.Lock()
        self.running=True
        #loop(csv_reader)
        t = threading.Thread(target=self.loop, args=(csv_reader,))
        t.start()

    def loop(self, csv_reader):
        for line in csv_reader:
            time.sleep(self.simplelog_interval)

            if self.running == False:
                return

            if not len(line) == 2:
                print("Simplelog skipping invalid line "+str(line))
                continue

            try:
                lat=float(line[0])
                lon=float(line[1])
            except ValueError:
                print("Simplelog skipping invalid line "+str(line))
                continue

            self.lock.acquire()
            self.position["lat"]=float(line[0])
            self.position["lon"]=float(line[1])
            self.position["valid"]=True
            self.lock.release()
       
            print("Line is "+str(line))
            
        print("Simplelog: FINISHED")


    def shutdown(self):
            self.running=False


    def getPosition(self):
        self.lock.acquire()
        p=self.position
        self.lock.release()
        return p
