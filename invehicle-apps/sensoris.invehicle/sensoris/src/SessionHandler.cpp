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

SessionHandler::SessionHandler(string sessionID, string vehicleID) {
   sessionIdentifier = make_shared<Identifier>(sessionID, vehicleID);
   dmHandler = make_shared<DataMessageHandler>(sessionIdentifier);
}

SessionHandler::~SessionHandler() {
   
}


string SessionHandler::getStringMessage() {
   shared_ptr<sensoris::protobuf::messages::data::DataMessage> dataMessage = dmHandler->getDataMessage();
   string encode_string;
   dataMessage->SerializeToString(&encode_string);
   return encode_string;
}

bool SessionHandler::startSession() {
   return dmHandler->startDataThreads();
}

bool SessionHandler::stopSession() {
   return dmHandler->stopDataThreads();
}  
