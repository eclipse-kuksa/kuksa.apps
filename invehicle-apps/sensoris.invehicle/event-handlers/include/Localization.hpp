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
#ifndef __LOCALIZATION_H__
#define __LOCALIZATION_H__
#include <stdio.h>
#include "sensoris/protobuf/categories/localization.pb.h"
#include "EventHandler.hpp"
#include "W3CClient.hpp"
using namespace std;


class Localization : public EventHandler {

 friend void* dataThreadDemo(void * arg);
 private:
    
    pthread_t startLocalization_thread;
    pthread_mutex_t* rwMutex;
    W3CClient* w3cclient;
    

  public:
    sensoris::protobuf::categories::localization::LocalizationCategory* loc_category;
    Localization();
    ~Localization();
    void merge(sensoris::protobuf::messages::data::EventGroup* evGroup);
    bool startDataThread();
    bool stopDataThread(); 
};
#endif
