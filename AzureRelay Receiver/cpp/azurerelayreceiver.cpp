/*
 * IPWorks MQ 2022 C++ Edition - Sample Project
 *
 * This sample project demonstrates the usage of IPWorks MQ in a 
 * simple, straightforward way. This is not intended to be a complete 
 * application. Error handling and other checks are simplified for clarity.
 *
 * Copyright (c) 2023 /n software inc. www.nsoftware.com
 */


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../../include/ipworksmq.h"


#define LINE_LEN 256

char input[LINE_LEN + 1];

const char* prompt(const char* prompt) {
	printf("%s: ", prompt);
	fgets(input, LINE_LEN, stdin);
	input[strlen(input) - 1] = '\0';
	if (strlen(input) == 0) {
		strncpy(input, "", LINE_LEN);
		input[LINE_LEN] = '\0';
	}
	return input;
}
const char* prompt(const char* prompt, const char* defaultVal) {
	printf("%s [%s]: ", prompt, defaultVal);
	fgets(input, LINE_LEN, stdin);
	input[strlen(input) - 1] = '\0';
	if (strlen(input) == 0) {
		strncpy(input, defaultVal, LINE_LEN);
		input[LINE_LEN] = '\0';
	}
	return input;
}

class MyAzurerelayreceiver : public AzureRelayReceiver
{
	virtual int FireSSLServerAuthentication(AzureRelayReceiverSSLServerAuthenticationEventParams* e) {
		//accept all
		e->Accept = true;
		return 0;
	}

	virtual int FireConnectionDataIn(AzureRelayReceiverConnectionDataInEventParams *e) {
		printf("Received data from [%i]: %s\r\n", e->ConnectionId, e->Text);
		printf("Echoing %s back to client\n",e->Text );
		this->SendText(e->ConnectionId, e->Text);
		return 0;
	}

	virtual int FireConnectionConnected(AzureRelayReceiverConnectionConnectedEventParams *e) {
		printf("[%i] connected\r\n", e->ConnectionId);
		return 0;
	}

	virtual int FireConnectionStatus(AzureRelayReceiverConnectionStatusEventParams *e) {
		printf("%s\n",e->ConnectionEvent);
		return 0;
	}

	virtual int FireConnectionDisconnected(AzureRelayReceiverConnectionDisconnectedEventParams *e) {
		printf("[%i] disconnected\r\n",e->ConnectionId);
		return 0;
	}

	virtual int FireDisconnnected(AzureRelayReceiverDisconnectedEventParams *e) {
		printf("Disconnected\n");
		return 0;
	}
};

int main()
{
	MyAzurerelayreceiver azurerelayreceiver1;

	printf("*************************************************************************\n");
	printf("* This demo shows how to use the AzureRelayReceiver component to accept *\n");
	printf("* connections and messages from an AzureRelaySender.                    *\n");
	printf("*************************************************************************\n\n");

	// Configure Azure Relay credentials
	azurerelayreceiver1.SetAccessKey(prompt("Set Access Key"));
	azurerelayreceiver1.SetAccessKeyName(prompt("Set Access Key Name", "RootManageSharedAccessKey"));
	azurerelayreceiver1.SetNamespaceAddress(prompt("Set Namespace Address"));
	azurerelayreceiver1.SetHybridConnection("hc1");
	printf("\n");

	azurerelayreceiver1.SetListening(true);

	while (true) azurerelayreceiver1.DoEvents();

	return 0;
}

