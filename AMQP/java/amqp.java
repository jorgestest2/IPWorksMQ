/*
 * IPWorks MQ 2022 Java Edition- Demo Application
 *
 * Copyright (c) 2023 /n software inc. - All rights reserved. - www.nsoftware.com
 *
 */

import java.io.*;

import ipworksmq.*;

public class amqp extends ConsoleDemo{

    public static void main(String[] args) {
        System.out.println("*******************************************************");
        System.out.println("* This is a demo for the IP*Works IoT AMQP Class.     *");
        System.out.println("* It allows simple message sending and receiving.     *");
        System.out.println("*******************************************************");
        Amqp amqp = new Amqp();
        
        
        try {
        //add listeners
        	amqp.addAmqpEventListener(new AmqpEventListener() {
    			
    			@Override
    			public void messageOutcome(AmqpMessageOutcomeEvent arg0) {
    				// TODO Auto-generated method stub
    				
    			}
    			
    			@Override
    			public void messageOut(AmqpMessageOutEvent arg0) {
    				// TODO Auto-generated method stub
    				
    			}
    			
    			@Override
    			public void messageIn(AmqpMessageInEvent arg0) {
    				System.out.println("\nMessage Received: "+amqp.getReceivedMessage().getValue());
    				
    			}
    			
    			@Override
    			public void log(AmqpLogEvent arg0) {
    				// TODO Auto-generated method stub
    				
    			}
    			
    			@Override
    			public void linkReadyToSend(AmqpLinkReadyToSendEvent arg0) {
    				// TODO Auto-generated method stub
    				
    			}
    			
    			@Override
    			public void error(AmqpErrorEvent arg0) {
    				// TODO Auto-generated method stub
    				
    			}
    			
    			@Override
    			public void disconnected(AmqpDisconnectedEvent arg0) {
    				if(arg0.statusCode!=0) {
						System.out.println("Error disconnecting: "+arg0.statusCode+" - "+arg0.description);
					}
    				System.exit(0);
    			}
    			
    			@Override
    			public void connectionStatus(AmqpConnectionStatusEvent arg0) {
    				// TODO Auto-generated method stub
    				
    			}
    			
    			@Override
    			public void connected(AmqpConnectedEvent arg0) {
    				System.out.println("Connected.");
    				
    			}
    			
    			@Override
    			public void SSLStatus(AmqpSSLStatusEvent arg0) {
    				// TODO Auto-generated method stub
    				
    			}
    			
    			@Override
    			public void SSLServerAuthentication(AmqpSSLServerAuthenticationEvent arg0) {
    				arg0.accept=true;
    				
    			}
    		});
        	
        //configure settings
        
        //set container id, ssl enabled and remote host
        amqp.setContainerId("ContainerId");
        boolean ssl = Boolean.valueOf(ask("Use SSL","?")=='y');
        amqp.setSSLEnabled(ssl);
        amqp.setRemoteHost(prompt("Remote Host"));
        
        //prompt for remote port
        String defPort = "5672";
        if(ssl) {
        	defPort = "5671"; 
    	}
        amqp.setRemotePort(Integer.valueOf(prompt("Remote Port",":",defPort)));
        
        //prompt for user and password
        amqp.setUser(prompt("User"));
        amqp.setPassword(prompt("Password"));
        
        //connect to host
        amqp.connectTo(amqp.getRemoteHost(), amqp.getRemotePort());
        
        //prompt for automatic or fetch flow
        char flow = ' ';
        while(flow!='a'&&flow!='f') {
    		flow = ask("Choose (a)utomatic or (f)etch flow",":","a/f");
        }
        if(flow=='f') {
        	amqp.setReceiveMode(Amqp.rmFetch);
            amqp.setFetchTimeout(5);
        }
        
        //set session id and create sender link with a unique link name and name of target node (at most once flow by default, settled = true)
        amqp.createSession("SessionId");
        amqp.createSenderLink("SessionId", "SenderLinkName", "TargetName");
        
        //create receiver link with existing session ID, unique link name, and name of the target node        
        amqp.createReceiverLink("SessionId", "ReceiverLinkName", "TargetName");
        
        char response = ask("\nWould you like to (s)end message, (f)etch message, or (q)uit","?","s/f/q");
        while(response!='q') {
        	if(response=='s') {
        		amqp.resetMessage();
        		amqp.getMessage().setValueType(17); //string
        		amqp.getMessage().setValue(prompt("Enter message"));
        		amqp.sendMessage("SenderLinkName");
        		System.out.println("Message sent.\n");
        	} else if(response=='f') {
        		System.out.println("Fetching message...");
        		try {
        		amqp.fetchMessage("ReceiverLinkName");
        		} catch(IPWorksMQException e) {
        			if(e.getCode()==201) {
        				System.out.println("Timeout. No message received.");
        			} else {
        				displayError(e);
        			}
        		}
        	}
        	response = ask("Would you like to (s)end message, (f)etch message, or (q)uit","?","s/f/q");
        }
        	
        } catch (Exception e) {
            displayError(e);
        }
    }
}
class ConsoleDemo {
  private static BufferedReader bf = new BufferedReader(new InputStreamReader(System.in));

  static String input() {
    try {
      return bf.readLine();
    } catch (IOException ioe) {
      return "";
    }
  }
  static char read() {
    return input().charAt(0);
  }

  static String prompt(String label) {
    return prompt(label, ":");
  }
  static String prompt(String label, String punctuation) {
    System.out.print(label + punctuation + " ");
    return input();
  }

  static String prompt(String label, String punctuation, String defaultVal)
  {
	System.out.print(label + " [" + defaultVal + "] " + punctuation + " ");
	String response = input();
	if(response.equals(""))
		return defaultVal;
	else
		return response;
  }

  static char ask(String label) {
    return ask(label, "?");
  }
  static char ask(String label, String punctuation) {
    return ask(label, punctuation, "(y/n)");
  }
  static char ask(String label, String punctuation, String answers) {
    System.out.print(label + punctuation + " " + answers + " ");
    return Character.toLowerCase(read());
  }

  static void displayError(Exception e) {
    System.out.print("Error");
    if (e instanceof IPWorksMQException) {
      System.out.print(" (" + ((IPWorksMQException) e).getCode() + ")");
    }
    System.out.println(": " + e.getMessage());
    e.printStackTrace();
  }
}




