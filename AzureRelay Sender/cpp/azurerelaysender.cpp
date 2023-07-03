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
bool dataInReceived;

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

class MyAzurerelaysender : public AzureRelaySender
{
	virtual int FireSSLServerAuthentication(AzureRelaySenderSSLServerAuthenticationEventParams *e)
	{
		if (!e->Accept)
		{
			printf("Server provided the following certificate: \n");
			printf("Subject: %s \n", e->CertSubject);
			printf("Issuer: %s \n", e->CertIssuer);
			printf("The following problems have been determined for this certificate:  %s \n", e->Status);
			printf("Would you like to continue or cancel the connection?  [y/n] ");

			char response[LINE_LEN];

			fgets(response, LINE_LEN, stdin);
			response[strlen(response) - 1] = '\0';
			if (strcmp(response, "y") == 0)
				e->Accept = true;
		}

		return 0;
	}

	virtual int FireDataIn(AzureRelaySenderDataInEventParams* e) {
		printf("Received %s: %s\r\n",e->Text);
		dataInReceived = true;
		return 0;
	}

	virtual int FireConnectionConnected(AzureRelayReceiverConnectionConnectedEventParams *e) {
		printf("[%i] connected\r\n", e->ConnectionId);
		return 0;
	}

	virtual int FireConnectionStatus(AzureRelaySenderConnectionStatusEventParams *e) {
		printf("%s\r\n",e->ConnectionEvent);
		return 0;
	}

	virtual int FireDisconnnected(AzureRelayReceiverDisconnectedEventParams* e) {
		printf("Disconnected\n");
		return 0;
	}
};

int main()
{
	MyAzurerelaysender azurerelaysender1;

	printf("*************************************************************************\n");
	printf("* This demo shows how to use the AzureRelaySender component to connect  *\n");
	printf("* and send messages to an AzureRelayReceiver.                           *\n");
	printf("*************************************************************************\n\n");

	// Configure Azure Relay credentials
	azurerelaysender1.SetAccessKey(prompt("Set Access Key"));
	azurerelaysender1.SetAccessKeyName(prompt("Set Access Key Name", "RootManageSharedAccessKey"));
	azurerelaysender1.SetNamespaceAddress(prompt("Set Namespace Address"));
	azurerelaysender1.SetHybridConnection("hc1");
	printf("\n");

	int ret_code = azurerelaysender1.Connect();

	if (ret_code)
	{
		printf("Error connecting: %i - %s\n", ret_code, azurerelaysender1.GetLastError());
		goto done;
	}

	char command[LINE_LEN];
	while (true)
	{
		dataInReceived = false;
		printf("\nPlease input command: \r\n- 1 Send Data \r\n- 2 Exit \r\n");
		printf(">");

		fgets(command, LINE_LEN, stdin);
		command[strlen(command) - 1] = '\0';

		if (!strcmp(command, "1"))
		{
			char text[LINE_LEN];
			printf("Please enter data to send: ");
			fgets(text, LINE_LEN, stdin);
			text[strlen(text) - 1] = '\0';
			ret_code = azurerelaysender1.SetDataToSend(text, strlen(text));
			if (ret_code)
			{
				printf("Sending failed: %i - %s\n", ret_code, azurerelaysender1.GetLastError());
			}
			else
			{
				printf("Waiting for response...\n");
				while (!dataInReceived)
					azurerelaysender1.DoEvents();
			}
		}
		else if (!strcmp(command, "2"))
		{
			goto done;
		}
		else
		{
			printf("Command not recognized.\n");
		}
	}

done:
	if (azurerelaysender1.GetConnected())
	{
		azurerelaysender1.Disconnect();
	}
	printf("Exiting... (press enter)\n");
	getchar();

	return 0;
}

