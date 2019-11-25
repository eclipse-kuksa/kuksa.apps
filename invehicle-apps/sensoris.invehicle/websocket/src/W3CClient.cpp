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

#include "W3CClient.hpp"

void LoadCertData(ssl::context& ctx) {
  std::string cert;
  std::string key;
  std::string line;

  {
    ifstream inputFile ("Client.pem");
    if (inputFile.is_open())
    {
      while ( getline (inputFile,line) )
      {
        cert.append(line);
        cert.append("\n");
      }
      inputFile.close();
    }
  }
  {
    ifstream inputFile ("Client.key");
    if (inputFile.is_open())
    {
      while ( getline (inputFile,line) )
      {
        key.append(line);
        key.append("\n");
      }
      inputFile.close();
    }
  }

  ctx.use_certificate_chain(
      boost::asio::buffer(cert.data(), cert.size()));

  ctx.use_private_key(
      boost::asio::buffer(key.data(), key.size()),
      boost::asio::ssl::context::file_format::pem);
}

string W3CClient::syncSendReceive(string message) {

   // Send the message
   ws->write(boost::asio::buffer(message));
   // This buffer will hold the incoming message
   boost::beast::multi_buffer b;
   // Read a message into our buffer
   ws->read(b); 
   stringstream ss; 
   ss << boost::beast::buffers(b.data());
   return ss.str();
   
   return "";
   
}

void W3CClient::syncSend(string message) {
   // Send the message
   ws->write(boost::asio::buffer(message));
}


void W3CClient::connect(string host, string port, bool isBinary) {
  
try {
      // The io_context is required for all I/O
      boost::asio::io_context ioc;

      tcp::resolver resolver{ioc};

      // The SSL context is required, and holds certificates
      ssl::context ctx{ssl::context::sslv23_client};
      // This holds the root certificate used for verification
      LoadCertData(ctx);
      ws = new websocket::stream<ssl::stream<tcp::socket>> {ioc, ctx};

      // Look up the domain name
      auto const results = resolver.resolve(host, port);

      // Make the connection on the IP address we get from a lookup
      boost::asio::connect(ws->next_layer().next_layer(), results.begin(), results.end());

      // Perform the SSL handshake
      ws->next_layer().handshake(ssl::stream_base::client);

      // Perform the websocket handshake
      ws->handshake(host, "/vss");
      
      // set binary connection
      ws->binary(isBinary);
         
   }
   catch(std::exception const& e)
   {
      std::cerr << "Error while connecting to server at "<< host << ":"<< port << "   " << e.what() << std::endl;
   }
}
   

string W3CClient::authorize(string token){
   jsoncons::json authreq;
   authreq["requestId"] = rand() % 999999;
   authreq["action"]= "authorize";
   authreq["tokens"] = token;
   stringstream ss; 
   ss << pretty_print(authreq);
  
   return syncSendReceive(ss.str());
}
   
jsoncons::json W3CClient::getValue(string vssPath) {
   jsoncons::json subreq;
   subreq["requestId"] = rand() % 999999;
   subreq["action"]= "get";
   subreq["path"] = vssPath;
   stringstream ss; 
   ss << pretty_print(subreq);
   string ret = syncSendReceive(ss.str());
   
   jsoncons::json retJson = jsoncons::json::parse(ret);

   if (retJson.has_key("value")) {
       return retJson["value"];
   }
   return "---";
}
   
string W3CClient::unsubscribe(long subscriptionId) {
   jsoncons::json unsubreq;
   unsubreq["requestId"] = rand() % 999999;
   unsubreq["action"]= "unsubscribe";
   unsubreq["subscriptionId"] = subscriptionId;
   stringstream ss; 
   ss << pretty_print(unsubreq);
   return syncSendReceive(ss.str());
}
