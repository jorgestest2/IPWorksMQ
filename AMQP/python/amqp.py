#
# IPWorks MQ 2022 Python Edition - Demo Application
#
# Copyright (c) 2023 /n software inc. - All rights reserved. - www.nsoftware.com
#

import sys
import string
from ipworksmq import *

input = sys.hexversion<0x03000000 and raw_input or input

amqp = AMQP()
msgReceived = False

def fire_message_in(e):
  print("Incoming Message from <%s>: %s\n" 
    % (e.link_name, amqp.received_message_value))
  global msgReceived
  msgReceived = True

def fire_ssl_server_authentication(e):
  e.accept = True

def get_input(prompt, default=""):
  if default != "":
    print(prompt + "[" + default + "]:")
  else:
    print(prompt + ":")
  result = input()
  if result == "":
    result = default
  return result

print("*******************************************************************************\n")
print("* This demo shows how to use the AMQP component to send and receive messages. *\n")
print("*******************************************************************************\n\n")

amqp.on_message_in = fire_message_in
amqp.on_ssl_server_authentication = fire_ssl_server_authentication

amqp.container_id = "ContainerId"

use_ssl = ""
use_ssl = get_input("Use SSL? (y/n)")
while use_ssl != "y" and use_ssl != "n":
  use_ssl = get_input("Use SSL? Please type 'y' or 'n'")
amqp.ssl_enabled = use_ssl == "y"

remote_host = get_input("Remote Host", "localhost")

remote_port = get_input("Remote Port", "5671" if amqp.ssl_enabled else "5672")

amqp.user = get_input("User")
amqp.password = get_input("Password")

try:
  amqp.connect_to(remote_host, int(remote_port))
except IPWorksMQError as error:
  print("Error connecting: %i - %s" % (error.code, error.message))
  exit()

amqp.create_session("SessionId")

try:
  amqp.create_sender_link("SessionId", "SenderLinkName", "TargetName")
except IPWorksMQError as error:
  print("Error creating sender link: %i - %s" % (error.code, error.message))
  exit()

amqp.receive_mode = 1
amqp.fetch_timeout = 5

try:
  amqp.create_receiver_link("SessionId", "ReceiverLinkName", "TargetName")
except IPWorksMQError as error:
  print("Error creating receiver link: %i - %s" % (error.code, error.message))
  exit()

command = ""
while command != "q":
  if command == "s":
    amqp.reset_message
    amqp.message_value_type = 17
    amqp.message_value = get_input("Enter message to send", "Hello!")
    try:
      amqp.send_message("SenderLinkName")
    except IPWorksMQError as error:
      print("Error sending message: %i - %s" % (error.code, error.message))
      exit()
    print("Message sent")
  elif command == "f":
    print("Fetching message...")
    try:
      amqp.fetch_message("ReceiverLinkName")
    except IPWorksMQError as error:
      if error.code == 201:
        print("Timeout - no message received")
      else:
        print("Error sending message: %i - %s" % (error.code, error.message))
        exit()
  command = get_input("Choose send message (s), fetch message (f), or quit (q)", "q")

