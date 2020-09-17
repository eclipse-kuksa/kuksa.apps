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


import csv
import time
import threading, queue, ssl
import asyncio, websockets, pathlib

position = { "valid":False, "lat":"0", "lon":"0", "alt":"0", "hdop":"0", "speed":"0" }
lock=threading.Lock()
RUNNING=False

class VSSProvider(threading.Thread):

    # Constructor
    def __init__(self, config):
        super(VSSClientComm, self).__init__()
        provider_config=config['Provider.kuksa_val']
        self.serverIP = provider_config.get('ip', "127.0.0.1")
        self.serverPort = provider_config.get('port', 8090)
        self.insecure = provider_config.get('insecure', false)
        self.interval=provider_config.getint('interval',1)
        self.cacertificate = provider_config.get('cacertificate', "CA.pem")
        self.certificate = provider_config.get('certificate', "Client.pem")
        self.keyfile = provider_config.get('key', "Client.key")
        self.token = provider_config.get('token', "token.json")
        print("Init kuksa_val provider...")
        if "Provider.kuksa_val" not in config:
            print("Provider.kuksa_val section missing from configuration, exiting")
            sys.exit(-1)
        
        if not self.insecure:
            context = ssl.create_default_context(ssl.Purpose.CLIENT_AUTH)
            context.load_cert_chain(certfile=self.certificate, keyfile=self.keyfile)
            context.load_verify_locations(cafile=self.certificate)
            try:
                with websockets.connect("wss://"+self.serverIP+":"+str(self.serverPort), ssl=context) as ws:
                    self.webSocket = ws
            except OSError as e:
                print("Disconnected!!" + str(e))
                exit(-1)
        else:
            try:
                async with websockets.connect("ws://"+self.serverIP+":"+str(self.serverPort)) as ws:
                    self.webSocket = ws
            except OSError:
                print("Disconnected!!" + str(e))
                exit(-1)
        self.authorize()
        global RUNNING
        self.RUNNING = True

    def stopComm(self):
        global RUNNING
        RUNNING = False

    def authorize(self):
        if os.path.isfile(self.token):
            with open(tokenOrFile, "r") as f:
                token = f.readline()
        else:
            token = tokenOrFile

        req = {}
        req["requestId"] = 1238
        req["action"]= "authorize"
        req["tokens"] = token
        jsonDump = json.dumps(req)
        self.webSocket.send(req)
        resp = self.webSocket.recv()
        print(resp)

    def getValue(self, path):
        req = {}
        req["requestId"] = 1234
        req["action"]= "get"
        req["path"] = path
        jsonDump = json.dumps(req)
        self.webSocket.send(jsonDump)
        resp = self.webSocket.recv()
        print(resp)

    def mainLoop(self):
        global RUNNING
        while RUNNING:
            
            global position, lock
            datetime.sleep(self.interval)


    # Thread function: Start the asyncio loop
    def run(self):
        loop = asyncio.new_event_loop()
        asyncio.set_event_loop(loop)
        loop.run_until_complete(self.mainLoop())


def initProvider(config):
    t = VSSProvider(config)
    t.start()
    return t

def shutdown():
    global RUNNING
    RUNNING=False


def getPosition():
    global position, lock
    lock.acquire()
    p=position
    lock.release()
    return p
