/*
 * ******************************************************************************
 * Copyright (c) 2019 Robert Bosch GmbH.
 *
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v2.0
 * which accompanies this distribution, and is available at
 * https://www.eclipse.org/org/documents/epl-2.0/index.php
 *
 *  Contributors:
 *      Robert Bosch GmbH - initial API and functionality
 * *****************************************************************************
 */
#include "SessionHandler.hpp"
#include <iostream>
#include "W3CClient.hpp"

#define LOOP_DELAY 1000000

// Command line agrs
string vehicle_cloud_host, vehicle_cloud_port, sessionID, vehicleID, w3c_server_host, w3c_server_port, w3c_jwt_token;

using namespace std;

int main(int argc, char* argv[])
{

   if(argc == 8) {
      vehicle_cloud_host = string(argv[1]);
      vehicle_cloud_port = string(argv[2]);
      sessionID = string(argv[3]);
      vehicleID = string(argv[4]);
      w3c_server_host = string(argv[5]);
      w3c_server_port = string(argv[6]);
      w3c_jwt_token = string(argv[7]); 
   } else {
           cerr<<"Usage ./sensoris-invehicle-api <VEHICLE CLOUD ADDR> <VEHICLE CLOUD PORT> <SESSION ID> <VIN> <W3C SERVER ADDR> <W3C SERVER PORT> <W3C JWT TOKEN>"<<endl;
           cerr << "This sensoris invehicle app gets the data from w3c visserver hence it is necessary to configure a running w3c server"<< endl;
           return -1; 
   }

   cout << "Start Sensoris GPS vehicle node" << endl;
   SessionHandler sessionHandl(sessionID, vehicleID);
   cout << "Starting data session ... "<< endl;
   sessionHandl.startSession();
   W3CClient *client = new W3CClient();
   // Connection with the sensoris vehicle cloud should carry binary data hence set to true. 
   client->connect(vehicle_cloud_host, vehicle_cloud_port, true);
   // Start a continuous loop.
   while(1) {
      string message = sessionHandl.getStringMessage();
      client->syncSend(message);
      usleep(LOOP_DELAY);
   }
}


