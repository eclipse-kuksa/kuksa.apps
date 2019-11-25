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
#ifndef __SESSIONHANDLER_H__
#define __SESSIONHANDLER_H__
#include "sensoris/protobuf/messages/data.pb.h"
#include "DataMessageHandler.hpp"
#include "Identifier.hpp"

using namespace std;

class SessionHandler {
  
  private:
      shared_ptr<DataMessageHandler> dmHandler;
      shared_ptr<Identifier> sessionIdentifier;
      
  public:

      SessionHandler(string sessionID, string vehicleID);
      ~SessionHandler();
      string getStringMessage();
      bool startSession();
      bool stopSession();   
};
#endif
