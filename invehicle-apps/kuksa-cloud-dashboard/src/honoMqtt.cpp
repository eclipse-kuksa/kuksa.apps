/*
 * ******************************************************************************
 * Copyright (c) 2018 Robert Bosch GmbH and others.
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
#include "honoMqtt.hpp"

using namespace std;
using namespace std::chrono;

#define QOS 1

const string PUB_TOPIC = "telemetry";

/**
 * A base action listener for pub.
 */
class action_listener_pub : public virtual mqtt::iaction_listener
{
protected:
	void on_failure(const mqtt::token& tok) override {
		cout << "Publish failure for token: "
			<< tok.get_message_id() << endl;
	}

	void on_success(const mqtt::token& tok) override {
		cout << "Publish success for token: "
			<< tok.get_message_id() << endl;
	}
};

/**
 * A base action listener for sub.
 */
class action_listener_sub : public virtual mqtt::iaction_listener
{
protected:
	void on_failure(const mqtt::token& tok) override {
		cout << "Subscribe failure for token: "
			<< tok.get_message_id() << endl;
	}

	void on_success(const mqtt::token& tok) override {
		cout << "Subscribe success for token: "
			<< tok.get_message_id() << endl;
	}
};


void HonoMqtt::connect (string honoAddr , string clientID, string userName, string password, int  offlineBufferSize , int connAlive) {
   
   cout << "Creating async mqtt client with persist of max msg = "<< offlineBufferSize << endl;
   p_Client = new mqtt::async_client (honoAddr, clientID, offlineBufferSize, nullptr);
   p_Client->set_callback(*this);
   mqtt::connect_options connOpts;
   connOpts.set_keep_alive_interval(seconds(connAlive)); // keep alive for seconds passed
   connOpts.set_clean_session(true);
   connOpts.set_user_name(userName);
   connOpts.set_password(password);
   connOpts.set_automatic_reconnect(true);  // reconnects automatically.
   top = new mqtt::topic(*p_Client, PUB_TOPIC, QOS, false);

   try {
         cout << "Connecting..." << endl;
	 p_Client->connect(connOpts)->wait();
         cout << "Connected." << endl;
   } catch (const mqtt::exception& exc) {
                cout << "Exception occured while connecting to hono"<< endl;
		cerr << exc.what() << endl;
		throw exc;
   }
}
 
void HonoMqtt::publish (string data) {
    try {
    cout << "Sending next message..." << endl;
    action_listener_pub listener;
    top->publish(std::move(data));
    cout << "Published." << endl;
    } catch (const mqtt::exception& exc) {
      cout << "Exception occured while publishing data"<< endl;
      cerr << exc.what() << endl;
      throw exc;
   }
    
}

void HonoMqtt::disconnect() {
   cout << "Disconnecting..." << endl;
   mqtt::token_ptr  conntok = p_Client->disconnect();
   conntok->wait();
   cout << "Disconnected" << endl;
}

void HonoMqtt::setMessageCB(void(*msgCB)(string)) {
   onMsg = msgCB;
}








