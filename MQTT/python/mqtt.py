#
# IPWorks MQ 2022 Python Edition - Demo Application
#
# Copyright (c) 2023 /n software inc. - All rights reserved. - www.nsoftware.com
#

import sys
import string
from ipworksmq import *

input = sys.hexversion<0x03000000 and raw_input or input

mqtt = MQTT()
msgReceived = False

def fire_message_in(e):
  print("Incoming message from <%s>: %s\n" % (e.topic, e.message.decode("UTF-8")))
  global msgReceived
  msgReceived = True

def fire_subscribed(e):
  print("Subscribed to %s.\n", e.topic_filter)

def get_input(default):
  result = input()
  if result == "":
    result = default
  return result


print("******************************************************************************\n")
print("* This demo shows how to use the MQTT component to publish and subscribe to  *\n")
print("* topics. The demo makes use of a publicly available test server.            *\n")
print("******************************************************************************\n\n")

mqtt.on_message_in = fire_message_in
mqtt.on_subscribed = fire_subscribed

mqtt.set_client_id("testClient")
mqtt.connect_to("test.mosquitto.org", 1883)

print("Please enter a topic to subscribe and publish to [nsoftware_test]:")
topic = get_input("nsoftware_test")
mqtt.subscribe(topic, 1)

print("Please enter a message to send [Hello MQTT!]:")
message = get_input("Hello MQTT!")
mqtt.publish_message(topic, 1, message)

while not msgReceived:
  mqtt.do_events()

print("Press any key to continue.")
input()


