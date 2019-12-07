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
#ifndef __IDENTIFIER_H__
#define __IDENTIFIER_H__
#include <stdio.h>

using namespace std;

class Identifier {

  private:
     // Internal mapping id.
     int mapID;

     // Sensoris identifiers
     string vehicleID;
     string sessionID;
     uint64_t messageID;

  public:

    inline string getSessionID() {
         return sessionID;
    };

    inline uint64_t getMessageID() {
         return messageID;
    };
     
    inline Identifier(string sessionid, string vehicleid) {
         sessionID = sessionid;
         vehicleID = vehicleid;
         messageID = 0;
    };

    inline uint64_t incrementMsgID() {
         messageID ++;
         return messageID;
    };

    inline string getVehicleID() {
         return vehicleID;
    };
     
    inline void setVehicleID(string id) {
         vehicleID = id;
    };

    inline void setSessionID(string id) {
         sessionID = id;
    };

    inline int getMapID() {
         return mapID;
    };
     
    inline void setMapID(int id) {
         mapID = id;
    };

};
#endif



