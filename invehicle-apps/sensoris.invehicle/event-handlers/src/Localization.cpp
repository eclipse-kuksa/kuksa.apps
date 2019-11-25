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
#include <unistd.h>
#include <ctime>
#include "Localization.hpp"
#include "W3CClient.hpp"
        

#define LATITUDE "Vehicle.Cabin.Infotainment.Navigation.CurrentLocation.Latitude"
#define LONGITUDE "Vehicle.Cabin.Infotainment.Navigation.CurrentLocation.Longitude"

#define W3C_HOST "localhost"
#define W3C_PORT "8090"
#define TOKEN "eyJhbGciOiJSUzI1NiIsInR5cCI6IkpXVCJ9.eyJzdWIiOiJFeGFtcGxlIEpXVCIsImlzcyI6IkVjbGlwc2Uga3Vrc2EiLCJhZG1pbiI6dHJ1ZSwiaWF0IjoxNTE2MjM5MDIyLCJleHAiOjE2MDkzNzI4MDAsInczYy12c3MiOnsiVmVoaWNsZS5PQkQuKiI6InJ3IiwiVmVoaWNsZS5DYWJpbi5JbmZvdGFpbm1lbnQuTmF2aWdhdGlvbi5DdXJyZW50TG9jYXRpb24uKiI6InJ3In19.QYs78CwwyOPnzXUZdHYbWzD7hwMWuNJWuNBTNYIBQqvkM7Df5CsIV_CvfiozSQybSPwlthnm4yJ7xgyC2yQTLh6vKKEIAlNjFcOep39b3fGEDDrRHkOyMJQuxYD5ELALKEPbakITi4QfqWozcZQNY9AdyYROKJqmTPlaYZmT0_mAQQFCIk7ji-udxtqTGtO6-WW5cDPkp0cqaDy1ZJusVUsRJGUkd942jgqbcVp98n7j4o8NvSQhZ6wGrG8Nc1tpTMUdXQmuMPA3Iv2P3cSMuBrHAY5I8511OFgIpB51gydKF4PaqTpirxCswm85iZq6so9CnMwh9I7VBO5GI6Nz_lnFfyeg1NEdRmSTEI6ucqLhjx54R2cT-fa6KIHcDKw63ZrXGT-dNyhX2px0Q5zM2zuy8phwLj6cXujCS8-uOmlysvmxwgh3oIUmtmP47x5ZLkFqaYuU2Ue-kiSr1lyleaEhQxSGykCJr6OibQ4gA6b5VIXn3mfkMWF8fse3Xm1D8jJ6UyE9zl55GhgvcAX9AlQYuqRTcaSvhZbzDmSO4pMda0wuHyHnB1DBiFcV9AtbPQsLraP2VVNGlKiWQkiJp66H0fkj1NPmQvCbbQ9v8YT-_DmAEvwnY9Mcx26FYodzC055lny4ThRXTCSmnJdxA05Av-BCjAGxPTinYxKCA1c"

extern string w3c_server_host, w3c_server_port, w3c_jwt_token;


long long latID = 0;
long long lonID = 0;
bool running;
uint64_t demoDelay = 1000;

Localization::Localization() {
   loc_category = new sensoris::protobuf::categories::localization::LocalizationCategory();
   running = false;
   rwMutex = new pthread_mutex_t();
   pthread_mutex_init(rwMutex, NULL);
}


Localization::~Localization() {
   delete loc_category;
}

  
// Needs to be overridden here.
void  Localization::merge(sensoris::protobuf::messages::data::EventGroup* evGroup) {
   pthread_mutex_lock (rwMutex); 
   evGroup->mutable_localization_category()->MergeFrom(*loc_category);
   pthread_mutex_unlock (rwMutex);
}

// Data thread
void* dataThread(void * arg) {
   cout << "W3C Client thread started "<< endl;
   W3CClient * client = new W3CClient();
   client->connect(w3c_server_host, w3c_server_port, false);
   cout << "Authorize the connection "<< endl;
   client->authorize(w3c_jwt_token);

   Localization* loc = (Localization*)arg;
   long lat, lon;
   cout << "Localization Thread entered continuous loop " << endl; 
   while(running) {
      jsoncons::json dataLat = client->getValue(LATITUDE);
      jsoncons::json dataLon = client->getValue(LONGITUDE);
      if(dataLat.as<string>() != "---" && dataLon.as<string>() != "---" ) {
         lat = dataLat.as<long>();
         lon = dataLon.as<long>();
         auto gps = loc->loc_category->add_vehicle_position_and_orientation();
         gps->mutable_envelope()->mutable_timestamp()->mutable_posix_time()->set_value(std::time(nullptr));
         gps->mutable_position_and_accuracy()->mutable_geographic_wgs84()->mutable_latitude()->set_value(lat);
         gps->mutable_position_and_accuracy()->mutable_geographic_wgs84()->mutable_longitude()->set_value(lon);
      } else {
         cout << "Localization Thread : No GPS found in W3C Server"<< endl;
      }
         usleep(1000000);
   }
   return nullptr;
}
  

bool Localization::startDataThread() {
        
   if (running) {
      cout << "localization: Data thread already running"<<endl;
      return false;
   }
        
   pthread_t startdata_thread;
   running = true;
   if(pthread_create(&startdata_thread, NULL, dataThread, this)) {
      cout << "Localization : Unable to start data thread "<< endl;
      return false;
   }
        
   return true;
}
    

bool Localization::stopDataThread() {

   running = false;
   if (pthread_join(startLocalization_thread, NULL) == 0)
      return true;
   else
      return false;
}



  
