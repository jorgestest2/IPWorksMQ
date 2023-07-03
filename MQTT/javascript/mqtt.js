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

function mqttprompt(){
  process.stdout.write('mqtt> ');
}

async function main() {
  try {
    const mqtt = new ipworksmq.mqtt();

    // Connection settings with default topic.
    // test.mosquitto.org is a publicly available test server for MQTT.
    let host = 'test.mosquitto.org';
    let port = 1883;
    let topic = 'nsoftware/test';
    let qos = 1;
    let topicUnset = true;

    // Event to read a line of input
    rl.on('line', async (line) => {
      if(line.toLowerCase() === 'quit')
      {
        console.log('Quitting');
        process.exit();
      }
      // Set the topic if it hasn't been set yet
      else if(topicUnset){
        // Use the default if they enter a blank line
        if (line !== '') {
          topic = line;
        }
        topicUnset = false;
        // Subscribe to the topic, then prompt for a message
        await mqtt.subscribe(topic, qos, async function(e){
            if(err) {
              console.log(e.description);
              return;
            }
        });
        console.log(`Subscribed to ${topic}.\r\nEnter messages to send. Send \'quit\' to quit.`);
        mqttprompt();
      }
      else
      {
        // Publish a message to the topic and then prompt again
        await mqtt.publishMessage(topic, qos, line, function(e){
          if(err) {
            console.log(e.description);
            return;
          }
          mqttprompt();
        });
      }
    });
  
    // Fires when a message is received
    mqtt.on('MessageIn', async function(e){
      console.log(`Message from ${e.topic} at QoS ${e.QOS}:`);
      console.log(`${e.message.toString()}`);
      mqttprompt();
    });
  
    // Connect to the host and then prompt for a topic
    console.log(`Connecting to ${host}`);
    await mqtt.connectTo(host, port, function(e){
      if(err) {
        console.log(e.description);
        return;
      }
    });
    console.log('Connected.\r\nEnter topic to subscribe and publish to [default: nsoftware/test]. Send \'quit\' to quit.');
    mqttprompt();
  }
  catch (e) {
    console.log("Error: " + e);
  }
}

function prompt(promptName, label, punctuation, defaultVal)
{
  lastPrompt = promptName;
  lastDefault = defaultVal;
  process.stdout.write(`${label} [${defaultVal}] ${punctuation} `);
}
