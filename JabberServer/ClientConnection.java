package com.bham.fsd.assignments.jabberserver;

import java.io.*; 
import java.net.*;
import java.util.ArrayList;

public class ClientConnection implements Runnable {
	private Socket clientSocket = null ;
	private JabberDatabase db = null ;
	String username = "";
	 ObjectOutputStream oos =null;
	 ObjectInputStream ois = null;
	 
	public ClientConnection(Socket clientSocket, JabberDatabase db) {
		this.clientSocket=clientSocket;
		this.db=db;	
		Thread t= new Thread();
		t.start();
		//System.out.println("Creating client connection");
	}
	public void SignIn(String txt) throws IOException
	{
		if(db.getUserID(txt)==-1) 
		{
			JabberMessage info = new JabberMessage("unknown-user");
			oos = new ObjectOutputStream(clientSocket.getOutputStream());
			oos.writeObject(info);
			oos.flush();
			
		}
		else
		{	username = txt;
		JabberMessage info = new JabberMessage("signedin");
		oos = new ObjectOutputStream(clientSocket.getOutputStream());
		oos.writeObject(info);
		oos.flush();
		}
	}
	public void run() {
		
		try {
			boolean close = false;
			//System.out.println("Running the client's request");
			ois = new ObjectInputStream(clientSocket.getInputStream());
		
				while(true&&!close)
				{
					JabberMessage request = (JabberMessage) ois.readObject();
					String message = request.getMessage();
					
					//System.out.println("Client's request is: "+message);
					String sub="";
					String sub1="";
					if(message.length()!=0)
					{
						if(message.indexOf(' ')!=-1)
						{
							sub = message.substring(0,message.indexOf(' '));
							sub1 = message.substring(message.indexOf(' ')+1);
						}
						else sub=message;
				
						switch(sub)
						{
						case "signin":
						{
							 SignIn(sub1);
							
						}break;
						case "register":
						{
							if(db.getUserID(sub1)==-1)
							{	username = sub1;
								db.addUser(sub1, sub1+"@gmail.com");	
								SignIn(username);
							}
							else
							{
								JabberMessage info = new JabberMessage("unknown-user");
								oos.writeObject(info);
								oos.flush();
							}
							
							
						}break;
						case "signout":
						{
							close=true;
							clientSocket.close();
						}break;
						
						case "timeline":
						{	if(username.equals(""))
						{
							System.out.println("No username");
						}
						else
						{
							ArrayList<ArrayList<String>> data  = db.getTimelineOfUserEx(username);
							JabberMessage info = new JabberMessage("timeline",data);
							oos.writeObject(info);
							oos.flush();
						}
							
						}break;
						case "users":
						{
							if(username.equals(""))
							{
								System.out.println("no username");
							}
							else
							{int userid = db.getUserID(username);
								ArrayList<ArrayList<String>> data  = db.getUsersNotFollowed(userid);
								JabberMessage info = new JabberMessage("users",data);
								oos.writeObject(info);
								oos.flush();
							}
						}break;
						case "post":
						{
							if(username.equals("")||sub1.equals(""))
							{
								System.out.println("Information missing");
							}
							else
							{
								db.addJab(username, sub1);
								JabberMessage info = new JabberMessage("posted");
								oos.writeObject(info);
								oos.flush();
							}
						}break;
						case "like":
						{
							if(username.equals("")||sub1.equals(""))
							{
								System.out.println("Information missing");
							}
							else
							{int userid = db.getUserID(username);
							int jabid = Integer.parseInt(sub1);
								db.addLike(userid, jabid);
								JabberMessage info = new JabberMessage("posted");
								oos.writeObject(info);
								oos.flush();
							}
						}break;
						case "follow":
						{
							if(username.equals("")||sub1.equals(""))
							{
								System.out.println("Information missing");
							}
							else
							{int userid=db.getUserID(username);
								db.addFollower(userid, sub1);
								JabberMessage info = new JabberMessage("posted");
								oos.writeObject(info);
								oos.flush();
							}
						}break;
						default : {close=true; }
					}
					
				}
					}
					
			
			
				
		}
		catch (Exception e)
		{
			e.printStackTrace();
		}
	finally {
            try {
                if (oos != null) {
                    oos.close();
                }
                if (ois != null) {
                    ois.close();
                   clientSocket.close();
                }
            }
            catch (IOException e) {
                e.printStackTrace();
            }
        }
     }
		
	}


