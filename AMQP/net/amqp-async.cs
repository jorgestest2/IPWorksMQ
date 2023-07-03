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
using System;
using nsoftware.async.IPWorksMQ;
using System.Threading.Tasks;

public class AMQPDemo
{
  private static Amqp amqp = new Amqp();
  private static bool msgReceived = false;

  static async Task Main(string[] args)
  {
    Console.WriteLine("*******************************************************************************\n");
    Console.WriteLine("* This demo shows how to use the AMQP component to send and receive messages. *\n");
    Console.WriteLine("*******************************************************************************\n\n");

    amqp.OnMessageIn += FireMessageIn;
    amqp.OnSSLServerAuthentication += FireSSLServerAuthentication;

    amqp.ContainerId = "ContainerId";

    var use_ssl = GetInput("Use SSL? (y/n)");
    while (use_ssl != "y" && use_ssl != "n")
    {
      use_ssl = GetInput("Use SSL? Please type 'y' or 'n'");
    }
    amqp.SSLEnabled = use_ssl == "y";

    var remote_host = GetInput("Remote Host", "localhost");
    var remote_port = GetInput("Remote Port", amqp.SSLEnabled ? "5671" : "5672");

    amqp.User = GetInput("User");
    amqp.Password = GetInput("Password");

    try
    {
      await amqp.ConnectTo(remote_host, int.Parse(remote_port));
    }
    catch (IPWorksIoTException error)
    {
      Console.WriteLine($"Error connecting: {error.Code} - {error.Message}");
      Environment.Exit(0);
    }

    await amqp.CreateSession("SessionId");

    try
    {
      await amqp.CreateSenderLink("SessionId", "SenderLinkName", "TargetName");
    }
    catch (IPWorksIoTException error)
    {
      Console.WriteLine($"Error creating sender link: {error.Code} - {error.Message}");
      Environment.Exit(0);
    }

    amqp.ReceiveMode = AmqpReceiveModes.rmFetch;
    amqp.FetchTimeout = 5;

    try
    {
      await amqp.CreateReceiverLink("SessionId", "ReceiverLinkName", "TargetName");
    }
    catch (IPWorksIoTException error)
    {
      Console.WriteLine($"Error creating receiver link: {error.Code} - {error.Message}");
      Environment.Exit(0);
    }

    var command = "";
    while (command != "q")
    {
      if (command == "s")
      {
        await amqp.ResetMessage();
        amqp.Message.ValueType = AMQPValueTypes.mvtString;
        amqp.Message.Value = GetInput("Enter message to send", "Hello!");

        try
        {
          await amqp.SendMessage("SenderLinkName");
        }
        catch (IPWorksIoTException error)
        {
          Console.WriteLine($"Error sending message: {error.Code} - {error.Message}");
          Environment.Exit(0);
        }
        Console.WriteLine("Message sent");
      }
      else if (command == "f")
      {
        Console.WriteLine("Fetching message...");
        try
        {
          await amqp.FetchMessage("ReceiverLinkName");
        }
        catch (IPWorksIoTException error)
        {
          if (error.Code == 201)
          {
            Console.WriteLine("Timeout - no message received");
          }
          else
          {
            Console.WriteLine($"Error sending message: {error.Code} - {error.Message}");
            Environment.Exit(0);
          }
        }
      }
      command = GetInput("Choose send message (s), fetch message (f), or quit (q)", "q");
    }
  }

  private static void FireMessageIn(object sender, AmqpMessageInEventArgs e)
  {
    Console.WriteLine($"Incoming Message from <{e.LinkName}>: {amqp.ReceivedMessage}\n");
    msgReceived = true;
  }

  private static void FireSSLServerAuthentication(object sender, AmqpSSLServerAuthenticationEventArgs e)
  {
    e.Accept = true;
  }


  private static string GetInput(string prompt, string defaultVal = "")
  {
    Console.Write($"{prompt}{(defaultVal != "" ? $"[{defaultVal}]" : "")}: ");
    var result = Console.ReadLine();
    if (string.IsNullOrEmpty(result))
    {
      result = defaultVal;
    }
    return result;
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