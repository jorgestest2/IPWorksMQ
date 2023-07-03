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
#define LINE_LEN 100

class MyXMPP : public XMPP
{
public:
	virtual int FireConnected(XMPPConnectedEventParams *e)
	{
		printf("Welcome! Connection established!\n");
		return 0;
	}
	virtual int FireDisconnected(XMPPDisconnectedEventParams *e)
	{
		printf("Disconnected\n");
		return 0;
	}
	virtual int FireMessageIn(XMPPMessageInEventParams *e)
	{
		printf("%s said : %s\n\n", e->From, e->MessageText);
		return 0;
	}

	virtual int FireSSLServerAuthentication(XMPPSSLServerAuthenticationEventParams *e)
	{
		e->Accept = true;
		return 0;
	}
};

int main(int argc, char* argv[])
{

	if (argc != 4) {

		fprintf(stderr, "usage: xmpp server username password\n\n");
		fprintf(stderr, "  server    the name or address of the XMPP server\n");
		fprintf(stderr, "  username  the user name used to authenticate to the XMPP server\n");
		fprintf(stderr, "  password  the password used to authenticate to the XMPP server\n");
		fprintf(stderr, "\nExample: xmpp talk.google.com username password\n\n");
		printf("Press enter to continue.");
		getchar();
	}
	else{

		MyXMPP xmpp;
		char buddy[80]; //users input
		char buffer[LINE_LEN + 1];
		int ret_code = 0;
		int buddyCount = 0;


		xmpp.SetIMServer(argv[1]);
		xmpp.SetSSLStartMode(SSL_AUTOMATIC);
		
		printf("Connecting...\n\n");
		ret_code = xmpp.ConnectTo(argv[2], argv[3]);
		if (ret_code) goto done;

		buddyCount = xmpp.GetBuddyCount();
		printf("Buddy list:\n");

		if (buddyCount != 0){

			for (int i = 0; i < buddyCount; i++)
			{
				printf("%d""%s""%s\n", i + 1, ") ", xmpp.GetBuddyId(i));
			}

			printf("\nSelect a buddy : ");
			fgets(buddy, 80, stdin);
			buddy[strlen(buddy) - 1] = '\0';

			printf("\nMessage: ");
			fgets(buffer, 80, stdin);
			buffer[strlen(buffer) - 1] = '\0';

			xmpp.SetMessageText(buffer);

			xmpp.SendMessage(xmpp.GetBuddyId(atoi(buddy) - 1));

			printf("\n\nReceiving responses, Ctrl-C to exit.\n");
			while (1)
			{
				xmpp.DoEvents();
			}

			printf("Disconnecting...\n\n");
			ret_code = xmpp.Disconnect();
			if (ret_code) goto done;

		}
		else
		{
			printf("No buddies found!");
		}

	done:

		if (ret_code)     // Got an error.  The user is done.
		{
			printf("\nError: %d", ret_code);
			if (xmpp.GetLastError())
			{
				printf(" \"%s\"\n", xmpp.GetLastError());
			}
		}
		fprintf(stderr, "\npress <return> to continue...\n");
		getchar();
		exit(ret_code);
		return 0;


	}

}



