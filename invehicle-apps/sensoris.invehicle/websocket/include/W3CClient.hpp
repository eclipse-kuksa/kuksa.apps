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
#ifndef __W3CCLIENT_H__
#define __W3CCLIENT_H__
#include <jsoncons/json.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/beast/websocket/ssl.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ssl/stream.hpp>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>


using tcp = boost::asio::ip::tcp;               // from <boost/asio/ip/tcp.hpp>
namespace ssl = boost::asio::ssl;               // from <boost/asio/ssl.hpp>
namespace websocket = boost::beast::websocket;  // from <boost/beast/websocket.hpp>
using namespace std;


class W3CClient {

private:
   websocket::stream<ssl::stream<tcp::socket>>* ws;
   string syncSendReceive(string message);
public:
   void connect(string host, string port, bool isBinary);
   jsoncons::json getValue(string vssPath);
   string unsubscribe(long subscriptionId);
   string authorize(string token);
   void syncSend(string message);
};
#endif


