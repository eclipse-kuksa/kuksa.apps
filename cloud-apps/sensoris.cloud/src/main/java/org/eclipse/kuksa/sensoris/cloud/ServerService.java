/*******************************************************************************
 * Copyright (C) 2019 Robert Bosch GmbH and Others.
 *  
 *  This program and the accompanying materials are made
 * available under the terms of the Eclipse Public License 2.0
 * which is available at https://www.eclipse.org/legal/epl-2.0/
 *  
 * SPDX-License-Identifier: EPL-2.0
 *  
 * Contributors:
 *  Robert Bosch GmbH
 ******************************************************************************/

package org.eclipse.kuksa.sensoris.cloud;

import java.net.InetSocketAddress;
import java.security.KeyStore;

import javax.net.ssl.KeyManagerFactory;
import javax.net.ssl.SSLContext;
import javax.net.ssl.TrustManagerFactory;

import org.java_websocket.server.DefaultSSLWebSocketServerFactory;



public class ServerService  {

	private ServerWebsocketImpl server;
	
	private static int PORT = 9000; 

	public void send(String response) {
		server.sendToAll(response);
	}

	
	public void init(String address, int portNum, boolean secure) throws Exception {
		String host = address;
		int port = portNum;
		server = new ServerWebsocketImpl(new InetSocketAddress(host, port));

		if (secure) {
			String STORETYPE = "JKS";
			String KEYSTORE = "keystore.jks";
			String STOREPASSWORD = "storepassword";
			String KEYPASSWORD = "keypassword";

			KeyStore ks = KeyStore.getInstance(STORETYPE);
			// File kf = new File(t );
			ks.load(this.getClass().getClassLoader().getResourceAsStream(KEYSTORE), STOREPASSWORD.toCharArray());

			KeyManagerFactory kmf = KeyManagerFactory.getInstance("SunX509");
			kmf.init(ks, KEYPASSWORD.toCharArray());
			TrustManagerFactory tmf = TrustManagerFactory.getInstance("SunX509");
			tmf.init(ks);

			SSLContext sslContext = null;
			sslContext = SSLContext.getInstance("TLS");
			sslContext.init(kmf.getKeyManagers(), tmf.getTrustManagers(), null);

			server.setWebSocketFactory(new DefaultSSLWebSocketServerFactory(sslContext));
		}

		System.out.println(" Initialized server.");

	}

	
	public void startServer() {
		server.start();
		System.out.println(" Server Started.");
	}

	public static void main(String[] args) {
		try {
			ServerService sensorisCloud = new ServerService();
			sensorisCloud.init("127.0.0.1", PORT , true);
			sensorisCloud.startServer();
		} catch (Exception e) {
			System.out.println("Error while staring websocket server" + e);
		}
	}
	

}
