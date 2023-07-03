/*
 * IPWorks MQ 2022 Java Edition - Sample Project
 *
 * This sample project demonstrates the usage of IPWorks MQ in a 
 * simple, straightforward way. This is not intended to be a complete 
 * application. Error handling and other checks are simplified for clarity.
 *
 * Copyright (c) 2023 /n software inc. www.nsoftware.com
 */

import java.io.*;
import ipworksmq.*;

public class mqtt extends ConsoleDemo{

    public static void main(String[] args) {
        System.out.println("*******************************************************");
        System.out.println("* This is a demo for the IP*Works MQ MQTT Class.      *");
        System.out.println("* It prompts the user for a topic and message, then   *");
        System.out.println("* subscribes to that topic and publishes the message. *");
        System.out.println("* The demo uses a publicly available test server.     *");
        System.out.println("*******************************************************");
        Mqtt mqtt = new Mqtt();
        try {
            // Set up event handlers
            mqtt.addMqttEventListener(new MqttEventListener() {
                @Override
                public void connected(MqttConnectedEvent mqttConnectedEvent) {

                }

                @Override
                public void connectionStatus(MqttConnectionStatusEvent mqttConnectionStatusEvent) {

                }

                @Override
                public void disconnected(MqttDisconnectedEvent mqttDisconnectedEvent) {

                }

                @Override
                public void error(MqttErrorEvent mqttErrorEvent) {

                }

                @Override
                public void log(MqttLogEvent mqttLogEvent) {

                }

                @Override
                public void messageAck(MqttMessageAckEvent mqttMessageAckEvent) {

                }

                @Override
                public void messageIn(MqttMessageInEvent mqttMessageInEvent) {
                    // Fires whenever a message is received
                    System.out.println(new String(mqttMessageInEvent.message)); // Print the contents of the message.
                }

                @Override
                public void messageOut(MqttMessageOutEvent mqttMessageOutEvent) {

                }

                @Override
                public void readyToSend(MqttReadyToSendEvent mqttReadyToSendEvent) {

                }

                @Override
                public void SSLServerAuthentication(MqttSSLServerAuthenticationEvent mqttSSLServerAuthenticationEvent) {

                }

                @Override
                public void SSLStatus(MqttSSLStatusEvent mqttSSLStatusEvent) {

                }

                @Override
                public void subscribed(MqttSubscribedEvent mqttSubscribedEvent) {

                }

                @Override
                public void unsubscribed(MqttUnsubscribedEvent mqttUnsubscribedEvent) {

                }
            });

            // Connect to the test server
            mqtt.connectTo("test.mosquitto.org", 1883);

            // Get a topic and subscribe to it
            String topic = prompt("Please enter a topic to subscribe to", ":", "nsoftware/test");

            // Subscribe at QoS Level 2
            mqtt.subscribe(topic, 2);

            Thread.sleep(1000); // Wait for a retained message

            // Get a message to post and post it to the topic
            String message = prompt("Please enter a message to post", ":", "Hello World!");
            // Publish the message at QoS level 2
            mqtt.publishMessage(topic, 2, message);

            Thread.sleep(1000); // Wait for the message to be published and returned
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




