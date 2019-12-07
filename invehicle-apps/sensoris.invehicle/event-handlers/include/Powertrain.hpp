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
#include <stdio.h>
#include "sensoris/protobuf/categories/powertrain.pb.h"
#include "EventHandler.hpp"
using namespace std;



class Powertrain : public EventHandler{
  
  private:
    sensoris::protobuf::categories::powertrain::PowertrainCategory* pt_category; 

  public:
    Powertrain();
    ~Powertrain();
    void merge(sensoris::protobuf::messages::data::EventGroup* evGroup);
    bool startDataThread();
    bool stopDataThread(); 
};
