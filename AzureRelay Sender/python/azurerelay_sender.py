#
# IPWorks MQ 2022 Python Edition - Demo Application
#
# Copyright (c) 2023 /n software inc. - All rights reserved. - www.nsoftware.com
#

import sys
import string
from ipworksmq import *

input = sys.hexversion<0x03000000 and raw_input or input


dataReceived = False

def fireError(e):
  print("ERROR: " %e.message)

def fireSSLServerAuthentication(e):
  if (not e.accept):
    print("Server provided the following certificate: \n")
    print("Subject: %s \n", e.cert_subject)
    print("Issuer: %s \n", e.cert_issuer)
    print("The following problems have been determined for this certificate:  %s \n", e.status)
    response = input("Would you like to continue or cancel the connection?  [y/n] ")
    if (response =="y"):
      e.accept = True

def fireConnectionStatus(e):
  print(e.connection_event)

def fireDataIn(e):
  global dataReceived
  dataReceived = True
  print ("Received: " + bytes.decode(e.text))

def fireConnectionConnected(e):
  pass

print("********************************************************************")
print("* This is a demo to show how to connect to an Azure Relay Service  *")
print("* in order to send and receive data.                               *")
print("********************************************************************")

try:
  azurerelaysender1 = AzureRelaySender()
  azurerelaysender1.on_data_in = fireDataIn
  azurerelaysender1.on_ssl_server_authentication = fireSSLServerAuthentication
  azurerelaysender1.on_connection_status = fireConnectionStatus
  azurerelaysender1.on_connection_connected = fireConnectionConnected
  
  azurerelaysender1.set_access_key(input("Set Access Key: "))
  azurerelaysender1.set_access_key_name(input("Set Access Key Name: "))
  azurerelaysender1.set_namespace_address(input("Set Namespace Address: "))
  azurerelaysender1.set_hybrid_connection("hc1")
  print("\n")
  
  azurerelaysender1.connect()
  print("Connected")
  while True: 
    command = int(input("Please input command: 1 [Send Data] or  2 [Exit]: "))
    if command == 1:
      send = input("Please enter data to send: ")
      azurerelaysender1.send(send.encode())
      dataReceived = False
      print("Now waiting for response...")
      while dataReceived == False:
        azurerelaysender1.do_events()
    else:
      azurerelaysender1.disconnect()
      break
      
except IPWorksMQError as e:
  print("ERROR: %s" % e.message)

