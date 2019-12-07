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
import java.nio.ByteBuffer;
import java.util.Collection;

import org.java_websocket.WebSocket;
import org.java_websocket.handshake.ClientHandshake;
import org.java_websocket.server.WebSocketServer;
import org.sensoris.messages.data.DataMessage;

import com.google.protobuf.InvalidProtocolBufferException;


public class ServerWebsocketImpl extends WebSocketServer {

	WebSocketServer server;
	
	
	public ServerWebsocketImpl(InetSocketAddress address) {
		super(address);
	}

	
	@Override
	public void onOpen(WebSocket conn, ClientHandshake handshake) {
		System.out.println("new connection to " + conn.getRemoteSocketAddress());
	}

	@Override
	public void onClose(WebSocket conn, int code, String reason, boolean remote) {
		System.out.println("closed " + conn.getRemoteSocketAddress() + " with exit code " + code + " additional info: " + reason);
	}

	@Override
	public void onMessage(WebSocket conn, String message) {
		System.out.println("received message from "	+ conn.getRemoteSocketAddress() + ": " + message);
	}
	
	@Override
	public void onMessage( WebSocket conn, ByteBuffer message ) {
		
		System.out.println("received binary message ");
		DataMessage dm;
		try {
			dm = DataMessage.parseFrom(message);
			System.out.println(dm.toString());
		} catch (InvalidProtocolBufferException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
	}

	@Override
	public void onError(WebSocket conn, Exception ex) {
		System.err.println("an error occured on connection :" + ex.getMessage());
	}


	public void sendToAll( String text ) {
		Collection<WebSocket> con = connections();
		synchronized ( con ) {
			for( WebSocket c : con ) {
				c.send( text );
			}
		}
	}


	@Override
	public void onStart() {
		// TODO Auto-generated method stub
		
	}
	
}
