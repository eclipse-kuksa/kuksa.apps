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
#ifndef __EVENTHANDLER_H__
#define __EVENTHANDLER_H__

#include <stdio.h>
#include <jsoncons/json.hpp>
#include "sensoris/protobuf/messages/data.pb.h"
using namespace std;


class EventHandler {
  
  public:
    virtual void merge(sensoris::protobuf::messages::data::EventGroup* evGroup) = 0;
    virtual bool startDataThread() = 0;
    virtual bool stopDataThread() = 0;
    
};

#endif
