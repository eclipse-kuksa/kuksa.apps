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
#include <string>
#include <iostream>
#include "DataMessageHandler.hpp"

using namespace std;

// Constructor
DataMessageHandler::DataMessageHandler(shared_ptr<Identifier> sessID) {
    sessionID = sessID;
    loc_cat = make_shared<Localization>();
    pt_cat = make_shared<Powertrain>();
}

//Destructor
DataMessageHandler::~DataMessageHandler() {
   
}

shared_ptr<sensoris::protobuf::messages::data::DataMessage> DataMessageHandler::getDataMessage() {

    auto dataMessage = make_shared<sensoris::protobuf::messages::data::DataMessage>();
    auto envelope = dataMessage->mutable_envelope();
    envelope->mutable_ids()->mutable_session_id()->set_value(sessionID.lock()->getSessionID());
    envelope->mutable_ids()->mutable_message_id()->set_value(sessionID.lock()->incrementMsgID());

    auto eventGroup = dataMessage->add_event_group();
    // Add more event categories to the data message object.
    loc_cat->merge(eventGroup);
    pt_cat->merge(eventGroup);
    
#ifdef DEBUG
    cout << "Data message before encoding/serializing  = " << dataMessage->DebugString() << endl;
#endif
    

    // Just for testing.
    string encode_string;
    // Test serializing.
    dataMessage->SerializeToString(&encode_string);
    // Test De-serializing.
    //auto dataMessageTest = make_shared<sensoris::protobuf::messages::data::DataMessage>();    
    //dataMessageTest->ParseFromString(encode_string);
    //cout << "De-serialized debug message= " << dataMessageTest->DebugString() << endl;

    return dataMessage;
}


bool DataMessageHandler::startDataThreads() {

   loc_cat->startDataThread();
   pt_cat->startDataThread();
   return true;
}

bool DataMessageHandler::stopDataThreads() {
   loc_cat->stopDataThread();
   pt_cat->stopDataThread();
   return true;
}
