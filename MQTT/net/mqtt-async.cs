/*
 * IPWorks MQ 2022 .NET Edition - Sample Project
 *
 * This sample project demonstrates the usage of IPWorks MQ in a 
 * simple, straightforward way. This is not intended to be a complete 
 * application. Error handling and other checks are simplified for clarity.
 *
 * Copyright (c) 2023 /n software inc. www.nsoftware.com
 */

using System.Collections.Generic;
﻿
using System;
using System.Threading.Tasks;
using nsoftware.async.IPWorksMQ;

class Program
{
  private static Mqtt mqtt;
  private static string topic;
  private static bool topicUnset = true;

  static async Task Main(string[] args)
  {
    mqtt = new Mqtt();
    mqtt.OnMessageIn += Mqtt_OnMessageIn;

    string host = "test.mosquitto.org";
    int port = 1883;
    topic = "nsoftware/test";
    int qos = 1;

    try
    {
      Console.WriteLine($"Connecting to {host}");
      await mqtt.ConnectTo(host, port);
      Console.WriteLine($"Connected.\n Enter topic to subscribe and publish to [default: nsoftware/test]. Type 'quit' to quit.");
      Prompt();

      string input;
      while ((input = Console.ReadLine()) != null)
      {
        if (input.ToLower() == "quit")
        {
          Console.WriteLine("Quitting");
          break;
        }
        else if (topicUnset)
        {
          if (!string.IsNullOrEmpty(input))
          {
            topic = input;
          }

          topicUnset = false;

          try
          {
            await mqtt.Subscribe(topic, qos);
            Console.WriteLine($"Subscribed to {topic}.\n Enter messages to send. Type 'quit' to quit.");
          }
          catch (Exception e)
          {
            Console.WriteLine(e.Message);
          }
        }
        else
        {
          try
          {
            await mqtt.PublishMessage(topic, qos, input);
          }
          catch (Exception e)
          {
            Console.WriteLine(e.Message);
          }
        }

        Prompt();
      }
    }
    catch (Exception e)
    {
      Console.WriteLine(e.Message);
    }
  }

  private static void Mqtt_OnMessageIn(object sender, MqttMessageInEventArgs e)
  {
    Console.WriteLine($"Message from {e.Topic} at QoS {e.QOS}: \"{e.Message}\"");
  }

  private static void Prompt()
  {
    Console.Write("mqtt> ");
  }
}


class ConsoleDemo
{
  public static Dictionary<string, string> ParseArgs(string[] args)
  {
    Dictionary<string, string> dict = new Dictionary<string, string>();

    for (int i = 0; i < args.Length; i++)
    {
      // If it starts with a "/" check the next argument.
      // If the next argument does NOT start with a "/" then this is paired, and the next argument is the value.
      // Otherwise, the next argument starts with a "/" and the current argument is a switch.

      // If it doesn't start with a "/" then it's not paired and we assume it's a standalone argument.

      if (args[i].StartsWith("/"))
      {
        // Either a paired argument or a switch.
        if (i + 1 < args.Length && !args[i + 1].StartsWith("/"))
        {
          // Paired argument.
          dict.Add(args[i].TrimStart('/'), args[i + 1]);
          // Skip the value in the next iteration.
          i++;
        }
        else
        {
          // Switch, no value.
          dict.Add(args[i].TrimStart('/'), "");
        }
      }
      else
      {
        // Standalone argument. The argument is the value, use the index as a key.
        dict.Add(i.ToString(), args[i]);
      }
    }
    return dict;
  }
}