/*
 * IPWorks MQ 2022 JavaScript Edition - Sample Project
 *
 * This sample project demonstrates the usage of IPWorks MQ in a 
 * simple, straightforward way. This is not intended to be a complete 
 * application. Error handling and other checks are simplified for clarity.
 *
 * Copyright (c) 2023 /n software inc. www.nsoftware.com
 */
 
const readline = require("readline");
const ipworksmq = require("@nsoftware/ipworksmq");

if(!ipworksmq) {
  console.error("Cannot find ipworksmq.");
  process.exit(1);
}
let rl = readline.createInterface({
  input: process.stdin,
  output: process.stdout
});

main();

async function main() {

	const amqp = new ipworksmq.amqp();
	let messageReceived = false;

	// Fires when a message is received
	amqp.on('MessageIn', async (e) => {
	  console.log(`Message received: ${await amqp.getReceivedMessage().getValue()}`);
	  messageReceived = true;
	})
	.on('SSLServerAuthentication', (e) => ( e.accept = true))
	.on('ConnectionStatus', (e) => (console.log(`Connection status update: ${e.connectionEvent}`)));
  
	let argv = process.argv;
	if (argv.length < 4) {

		console.log("Usage: node amqp.js [option] action host");
		console.log("  Options: ");
		console.log("    -u      the user identifier to use for login to server (default is none)");
		console.log("    -p      the password to log in (default is none)");
		console.log("    -rp     the remote port to connect to on the remote host (default is 5672, or 5671 for ssl)");
		console.log("    -s      sets ssl enabled to true (default is false)");
		console.log("    -f      [only used when sending] sets receive mode to fetch while sending messages (default is automatic)");
		console.log("    -m      [only used when sending] the message to send (default is 'Hello, AMQP!'");
		console.log("  action    (send/fetch) action to perform; sending a message or fetching a message");
		console.log("  host      the domain name or IP address of the remote host to connect to");
		console.log("Examples: node amqp.js -u username -p password -m \"Hello, AMQP\" send [server address]"); //send with defaults: port 5672, no ssl, automatic mode
		console.log("          node amqp.js -u username -p password -rp 5670 -s -f -m \"Hello, AMQP\" send [server address]"); //send with options: port 5670, ssl enabled, fetch mode
		console.log("          node amqp.js -u username -p password fetch [server address]"); //fetch with defaults: port 5672, no ssl
		console.log("          node amqp.js -u username -p password -rp 5670 -s fetch [server address]"); //fetch with options: port 5670, ssl enabled
		process.exit();
	}
  
	//configure settings
	let message = "Hello, AMQP!";
	let remotePort = 5672;
	for (i = 0; i < argv.length; i++) {
	  if (argv[i].startsWith("-")) {
		if (argv[i] === "-u") amqp.setUser(argv[i + 1]); // args[i+1] corresponds to the value of argument [i]
		if (argv[i] === "-p") amqp.setPassword(argv[i + 1]);
		if (argv[i] === "-rp") {
		  remotePort = parseInt(argv[i + 1]);
		  amqp.setRemotePort(remotePort);
		}
		if (argv[i] === "-s") amqp.setSSLEnabled(true);
		if (argv[i] === "-f") amqp.setReceiveMode(1); //fetch mode = 1, default is automatic
		if (argv[i] === "-m") message = argv[i + 1];
	  }
	}
  
	try {
	  amqp.setContainerId("ContainerId"); //set unique containerid
	  await amqp.connect(argv[argv.length - 1], remotePort);
	  await amqp.createSession("SessionName"); //create session with session name
	  await amqp.createSenderLink("SessionName", "SenderLinkName", "TargetName"); //create sender link
	  if (argv[argv.length - 2] === "fetch") { //if action is fetch
		amqp.setReceiveMode(1); //fetch mode = 1, default is automatic
	  }
	  await amqp.createReceiverLink("SessionName", "ReceiverLinkName", "TargetName");
	} catch (error) {
	  console.log(`Error connecting: ${error.message}`);
	  process.exit();
	}
  
	if (argv[argv.length - 2] === "send") { //if action is send
	  //send message
	  try {
		amqp.setMessage(new ipworksmq.AMQPMessage());
		amqp.getMessage().setValueType(17); //string type
		await amqp.getMessage().setValue(message);
		await amqp.sendMessage("SenderLinkName"); //send message using same sender link
		console.log("Message sent.");
		if (amqp.getReceiveMode() === 0) { //if receive mode is auto
		  while (!messageReceived) { //wait until message received and prints
			await amqp.doEvents();
		  }
		}
	  } catch (error) {
		console.log(`Error sending message: ${error.message}`);
	  }
	  await amqp.disconnect();
	  process.exit();
	} else {
	  //fetch message
	  try {
		amqp.setFetchTimeout(5); //set timeout to 5 seconds
		console.log("Fetching message...");
		await amqp.fetchMessage("ReceiverLinkName"); //fetch message using same receiver link
	  } catch (error) {
		console.log(`Error fetching message: ${error.message}`);
	  }
	  await amqp.disconnect();
	  process.exit();
	}
}


function prompt(promptName, label, punctuation, defaultVal)
{
  lastPrompt = promptName;
  lastDefault = defaultVal;
  process.stdout.write(`${label} [${defaultVal}] ${punctuation} `);
}
