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

#ifndef __DMHANDLER_H__
#define __DMHANDLER_H__

#include <stdio.h>
#include <list>
#include "sensoris/protobuf/messages/data.pb.h"
#include "Localization.hpp"
#include "Powertrain.hpp"
#include "Identifier.hpp"

using namespace std;
class DataMessageHandler {

  private:
     weak_ptr<Identifier> sessionID;
     shared_ptr<Localization> loc_cat;
     shared_ptr<Powertrain> pt_cat;
     

  public:
     DataMessageHandler(shared_ptr<Identifier> sessID);
     ~DataMessageHandler();
     shared_ptr<sensoris::protobuf::messages::data::DataMessage> getDataMessage();
     bool startDataThreads();
     bool stopDataThreads();


}; 

#endif
