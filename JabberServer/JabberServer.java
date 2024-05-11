package com.bham.fsd.assignments.jabberserver;

import java.io.*; 
import java.net.*;

public class JabberServer implements Runnable {

	private static final int PORT_NUMBER= 44444;
	private static ServerSocket serverSocket;
	
	public JabberServer() 
	{
		try {
			serverSocket = new ServerSocket(PORT_NUMBER);
			serverSocket.setReuseAddress(true);
			//System.out.println("Started server");
			serverSocket.setSoTimeout(3000000);
			new Thread(this).start();
			//System.out.println("Started thread");
		}
		catch(Exception e)
		{
			e.printStackTrace();
		}
		
	}

	public void run() 
	{
		while (true) {
			Socket connectionSocket=null;
			try {
				connectionSocket = serverSocket.accept();
				//System.out.println("Started new client");
				ClientConnection client = new ClientConnection(connectionSocket, new JabberDatabase());
				client.run();
				Thread.sleep(100);
			}
		
		catch (IOException e)
		{
			e.printStackTrace();
		}
			catch (InterruptedException e) {
			
			e.printStackTrace();
			}
			
	}
	}
	
	public static void main(String args[]) throws Exception
	{  
			JabberServer jab = new JabberServer();
			
	
	}
}
