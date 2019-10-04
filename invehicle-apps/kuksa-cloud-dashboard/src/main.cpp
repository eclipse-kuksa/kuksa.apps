/*
 * ******************************************************************************
 * Copyright (c) 2018 Robert Bosch GmbH and others.
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
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <fstream> 
#include <iostream>
#include <unistd.h>
#include <json.hpp>
#include <sys/time.h>  
#include "client_wss.hpp"
#include "honoMqtt.hpp"
#include "emailHTTP.hpp"


// delay between sending each signals in microseconds
#define SENDDELAY 50000
#define MAX_ERRORS 10

using namespace std;

using WssClient = SimpleWeb::SocketClient<SimpleWeb::WSS>;
using namespace jsoncons;
using jsoncons::json;

const std::string CLIENT_ID = "cloud-dashboard";
const string PUB_TOPIC = "telemetry";

std::string query;

// WSS connection to w3c-server settings
shared_ptr<WssClient::Connection> connection = NULL;
string url = "localhost:8090/vss";

// CMD line args
char honoAddress[256];
char honoPort[32];
char honoDevice[256];
char honoPassword[256];
char TOKEN[2048];
char emailServerAddress[256];
char emailServerPort[64];

// E-mail settings
#define USER_EMAIL_FILE "/mnt/email"
bool mailsent[MAX_ERRORS];
string emailID ="";
string emailTemplate;

// MQTT Buffer settings
int  offlineBufferSize = 1024; // Max number of msgs to be buffered. This is default can be modified using cml line
int connAlive = 10 * 60; // keep connection alive for 10 mins this also means the bufferening will happen for max 10 mins. Can be modified useing cmd line 

// MQTT Adapter connection for Hono.
HonoMqtt* honoConn;

// callback method for receiving messages from hono.
void onMessageFromHono(string message) {
    cout<<"Publish msg from Hono = " << message << endl;
}

// get current time in ms
long int currentTimeMillis() {
   struct timeval tp;
   gettimeofday(&tp , NULL);
   return ((tp.tv_sec*1000) + (tp.tv_usec / 1000));
}


string readEmailAddr() {

    std::ifstream ifs (USER_EMAIL_FILE, std::ifstream::in);
    
   if( !ifs.is_open()) {
       cout << "User E-Mail iD not configured!!" << endl;
       return ""; 
   }
 

    string email;
    getline (ifs,email);
    ifs.close();
    cout <<"User Email= " << email << endl;
    return email;

}


void sendEmailToOwner( string resp) {
   
    json root;
    root = json::parse(resp);
    string dtccode;


   if(!root.has_key("requestId")){
      return;
   } else {
     int reqID = root["requestId"].as<int>();
     if (reqID == 1236) {
         reqID = 0;
         dtccode = "P0471";
     } else if(reqID == 1237) {
         reqID = 1;
         dtccode = "P0472";
     } else if(reqID == 1238) {
         reqID = 2;
         dtccode = "P0473";
     } else {
          return;
     }
   
   
   string emailAddr =  readEmailAddr();
  
   if(emailID != emailAddr) {
      cout << "Email Address changed to " << emailAddr << endl;
      for( int i = 0; i<MAX_ERRORS ; i++) {
          mailsent[i] = false;
      } 
      emailID = emailAddr;
      
   }

   if(mailsent[reqID] == true) {
       cout << "Email already sent to this user for this error!" <<endl;
       return;
   }

   if( emailID == "") {
      cout << "Configure E-mail Address of the User. This app is useless without it!" << endl;
      return;
   }

    string action = root["action"].as<string>();
    
    if( action == "authorize") {
        return;
    } else if ( root.has_key("error")) {
        cout << " Error response from server " <<  root["error"].as<string>() << endl;
        return;
    }
  
    if( !root.has_key("value")) {
        cout << " No value from server found" <<endl;
        return;
    } else if ( root["value"].as<string>() == "---") {
        cout << " No value set !! server has returned --- for"<< root["path"].as<string>() <<endl;
        return;
    }    

    if( !root.has_key("value")) {
        cout << " No value returned from server"<< endl;
        return;
    } 


    std::string value = root["value"].as<string>();
   
    
    if((reqID > -1 && reqID < MAX_ERRORS) && value == "true") {
        sendEmail(emailID, dtccode);
        mailsent[reqID] = true;
    }
  }
}



// Send msgs to Hono
void sendToHono( string resp) {

    cout << "Response >> " << resp << endl;
    json root;
    root = json::parse(resp);
    string action = root["action"].as<string>();

    if( action == "authorize") {
        return;
    } else if ( root.has_key("error")) {
        cout << " Error response from server " <<  root["error"].as<string>() << endl;
        return;
    }
  
    if( !root.has_key("value")) {
        cout << " No value from server found" <<endl;
        return;
    } else if ( root["value"].as<string>() == "---") {
        cout << " No value set !! server has returned --- for"<< root["path"].as<string>() <<endl;
        return;
    }    

    if( !root.has_key("value")) {
        cout << " No value returned from server"<< endl;
        return;
    } 

    std::string value = root["value"].as<string>();
       
    int reqID = root["requestId"].as<int>();
    string signal;
    json valJson;
   
    
    long int now = currentTimeMillis();
    cout << "timestamp "<< now << endl;
    valJson["time"] = now;
    
    if(reqID == 1234) {
       signal = "RPM";
       int val = stoi(value, nullptr, 10);
       valJson[signal] = val;
    } else if (reqID == 1235) {
       signal = "SPEED";
       int val = stoi(value, nullptr, 10);
       valJson[signal] = val;
    } else if (reqID == 1239) {
       signal = "FUEL";
       int val = stoi(value, nullptr, 10);
       valJson[signal] = val;
    } else if (reqID == 2222) {
       signal = "CATTEMP";
       std::string::size_type sz;
       float val = stof(value, &sz);
       valJson[signal] = val;
    } else if (reqID == 2223) {
       signal = "COOLTEMP";
       int val = stoi(value, nullptr, 10);
       valJson[signal] = val;
    } else if (reqID == 2224) {
       signal = "THROTTLEPOS";
       int val = stoi(value, nullptr, 10);
       valJson[signal] = val;
    } else if (reqID == 2225) {
       signal = "ACCPEDALPOS";
       int val = stoi(value, nullptr, 10);
       valJson[signal] = val;
    }
       else if (reqID == 1240) {
       signal = "GPS";
       json gps_value = json::parse(value);
       value = gps_value["Vehicle.Cabin.Infotainment.Navigation.CurrentLocation.Latitude"].as<string>();
       if (value == "---") {
          return;
       }
       value = value + ",";
       value = value + gps_value["Vehicle.Cabin.Infotainment.Navigation.CurrentLocation.Longitude"].as<string>();
       valJson[signal] = value;
       cout << "GPS data = " << value << endl;
    }


    try {
       honoConn->publish(valJson.as<string>());
    } catch ( mqtt::exception& e) {
       cout << "Exception occured while publishing msg " << e.what() << endl;
    }
}


void* sendSensorValues (void* arg) {
  
  usleep(SENDDELAY);
  auto send_stream = make_shared<WssClient::SendStream>();

  // send Authorize request.
   json authreq;
   authreq["requestId"] = rand() % 999999;
   authreq["action"]= "authorize";
   authreq["tokens"] = string(TOKEN);
   stringstream ss; 
   ss << pretty_print(authreq);
   *send_stream << ss.str();
   connection->send(send_stream); 


   string honoAddr = string(honoAddress) +":" + string(honoPort);
   honoConn = new HonoMqtt();
   try {
      cout << honoAddr << endl;
      string userName = string(honoDevice) + "@DEFAULT_TENANT";
      string password(honoPassword);
      honoConn->connect(honoAddr, CLIENT_ID, userName, password, offlineBufferSize, connAlive);
      honoConn->setMessageCB(onMessageFromHono);
   } catch ( mqtt::exception& e) {

   }

  // send data to hono instance.
  while(1) {
   string rpm_req = "{\"action\": \"get\", \"path\": \"Vehicle.OBD.EngineSpeed\", \"requestId\": 1234 }";

    
    *send_stream << rpm_req;
    connection->send(send_stream);    


    usleep(SENDDELAY);  
    string vSpeed_req = "{\"action\": \"get\", \"path\": \"Vehicle.OBD.Speed\", \"requestId\": 1235 }";
    
    *send_stream << vSpeed_req;
    connection->send(send_stream); 
    

    usleep(SENDDELAY); 

    string fuel_req = "{\"action\": \"get\", \"path\": \"Vehicle.OBD.FuelLevel\", \"requestId\": 1239 }";
    
    *send_stream << fuel_req;
    connection->send(send_stream);

    usleep(SENDDELAY); 

    string vlat_req = "{\"action\": \"get\", \"path\": \"Vehicle.Cabin.Infotainment.Navigation.CurrentLocation\", \"requestId\": 1240 }";
    *send_stream << vlat_req;
    connection->send(send_stream);

    usleep(SENDDELAY);

    string cat_req = "{\"action\": \"get\", \"path\": \"Vehicle.OBD.Catalyst.Bank1.Temperature1\", \"requestId\": 2222 }";
    
    *send_stream << cat_req;
    connection->send(send_stream); 


    usleep(SENDDELAY);  
    string coolant_req = "{\"action\": \"get\", \"path\": \"Vehicle.OBD.CoolantTemperature\", \"requestId\": 2223 }";
    
    *send_stream << coolant_req;
    connection->send(send_stream); 


    usleep(SENDDELAY);  
    string throttle_req = "{\"action\": \"get\", \"path\": \"Vehicle.OBD.ThrottlePosition\", \"requestId\": 2224 }";
    
    *send_stream << throttle_req;
    connection->send(send_stream); 



    usleep(SENDDELAY);  
    string accPos_req = "{\"action\": \"get\", \"path\": \"Vehicle.OBD.AcceleratorPositionD\", \"requestId\": 2225 }";
    
    *send_stream << accPos_req;
    connection->send(send_stream); 
  }
}


void* startWSClient(void * arg) {

  WssClient client(url , true, "Client.pem", "Client.key", "CA.pem");

  client.on_message = [](shared_ptr<WssClient::Connection> connection, shared_ptr<WssClient::Message> message) {
    string msg = message->string();
    sendToHono(msg);
    sendEmailToOwner(msg);
  };

  client.on_open = [](shared_ptr<WssClient::Connection> conn) {
    cout << "Connection with server at " << url << " opened" << endl;
    connection = conn;
       pthread_t honoConnect_thread;
        /* create the sensor values thread. */
        if(pthread_create(&honoConnect_thread, NULL, &sendSensorValues, NULL )) {

         cout<<"Error creating send sensor values thread"<<endl;
         return 1;

        }
  };

  client.on_close = [](shared_ptr<WssClient::Connection> /*connection*/, int status, const string & /*reason*/) {
    cout << "Connection closed with status code " << status << endl;
    connection = NULL;
  };

  // See http://www.boost.org/doc/libs/1_55_0/doc/html/boost_asio/reference.html, Error Codes for error code meanings
  client.on_error = [](shared_ptr<WssClient::Connection> /*connection*/, const SimpleWeb::error_code &ec) {
    cout << "Error: " << ec << ", message: " << ec.message() << endl;
  };

   client.start();
}


/**
 * @brief  Test main.
 * @return
 */
int main(int argc, char* argv[])
{

   if(argc == 10) {
      strcpy(honoAddress , argv[1]);
      char honoPortStr[16];
      strcpy(honoPort , argv[2]); 
      strcpy(honoDevice , argv[3]);
      strcpy(honoPassword , argv[4]);
      offlineBufferSize = stoi(argv[5],nullptr,10);
      connAlive = stoi(argv[6], nullptr, 10);
      strcpy(TOKEN, argv[7]);
      strcpy(emailServerAddress , argv[8]);
      strcpy(emailServerPort , argv[9]);
   } else {
      cerr<<"Usage ./dashboard <HONO-MQTT_IP_ADDR> <HONO_MQTT_PORT> <HONO_DEVICE_NAME> <HONO_PASSWORD> <BUFFER_SIZE> <BUFFER_TIME> <JWT_TOKEN> <EMAIL_SERVER_IP> <EMAIL_SERVER_PORT>"<<endl;
      return -1; 
   }

   pthread_t startWSClient_thread;

   /* create the web socket client thread. */
   if(pthread_create(&startWSClient_thread, NULL, &startWSClient, NULL )) {
      cout << "Error creating websocket client run thread"<<endl;
      return 1;
   }
   while (1) { usleep (1000000); };
}



