import SwiftUI
import IPWorksMQ

struct ShowMes: Identifiable {
  var id: String { message }
  let message: String
  var title: String = "Exception"
}
struct ContentView: View, XMPPDelegate {
  func onBuddyUpdate(buddyIdx: Int32) {}
  func onConnected(statusCode: Int32, description: String) {
    outputRes+="Connected\n"
  }
  func onConnectionStatus(connectionEvent: String, statusCode: Int32, description: String) {}
  func onDisconnected(statusCode: Int32, description: String) {
    outputRes+="Disconnnected\n"
  }
  func onEndTransfer(direction: Int32, fileId: String, filename: String, success: Bool) {}
  func onError(errorCode: Int32, description: String) {}
  func onIQ(iq: String, id: String, from: String, iqType: String, ignore: inout Bool) {}
  func onMessageIn(messageId: String, from: String, domain: String, resource: String, messageType: Int32, subject: String, messageThread: String, messageText: String, messageHTML: String, other: String) {
    outputRes+="\(from): \(messageText)\n"
  }
  func onPITrail(direction: Int32, pi: String) {}
  func onPresence(user: String, domain: String, resource: String, availability: Int32, status: String) {}
  func onReadyToSend() {}
  func onSSLServerAuthentication(certEncoded: Data, certSubject: String, certIssuer: String, status: String, accept: inout Bool) {}
  func onSSLStatus(message: String) {}
  func onStartTransfer(direction: Int32, fileId: String, user: String, domain: String, resource: String, filename: inout String, datetime: String, size: Int64, accept: inout Bool) {}
  func onSubscriptionRequest(from: String, domain: String, accept: inout Bool) {}
  func onSync() {}
  func onTransfer(direction: Int32, fileId: String, filename: String, bytesTransferred: Int64, percentDone: Int32, text: Data, cancel: inout Bool) {}
  
  var client = XMPP()
  var documentsPath = NSSearchPathForDirectoriesInDomains(.documentDirectory, .userDomainMask, true)[0] + "/";
  @State private var server: String = ""
  @State private var username: String = ""
  @State private var password: String = ""
  @State private var message: String = ""
  @State private var outputRes: String = ""
  @State private var selectedBuddy: Int = 0
  @State private var connected = false
  @State private var showMessage: ShowMes?
  
  private let buddyPickerStyle = PopUpButtonPickerStyle()
  
  func connectedChange() -> String
  {
    if (connected)
    {
      return "Disconnect"
    }
    else
    {
      return "Connect"
    }
  }
  
  var body: some View {
    VStack(alignment: .center)
    {
      Text("This demo shows how to use the XMPP module to create a simple XMPP client for instant messaging.").foregroundColor(Color.blue)
      Group {
        HStack{
          Text("Server:")
          TextField("Enter remote host", text: $server)
        }
        HStack{
          Text("Username:")
          
          TextField("Enter username", text: $username)
        }
        HStack{
          Text("Password:")
          
          SecureField("Enter password", text: $password)
        }
        connectButton()
      }
      
      Section {
        Picker(selection: $selectedBuddy, label: Text("Select buddy")) {
          ForEach((0..<client.buddies.count), id: \.self) {
            Text(client.buddies[$0].id)
          }
        }
        .pickerStyle(buddyPickerStyle)
      }
      .disabled(connected == false)
      
      Text("Output:")
      TextEditor(text: $outputRes)
        .border(Color.black, width: 1)
      
      Group
      {
        Text("Message:")
        
        TextField("Enter message", text: $message)
        sendButton()
      }
    }
    .alert(item: $showMessage) { show in
      Alert(title: Text(show.title), message: Text(show.message), dismissButton: .default(Text("Ok")))
    }.padding(.all, 8.0)
  }
  
  @ViewBuilder
  private func connectButton() -> some View {
    Button(action:
            {
      //client.runtimeLicense = "";
      client.delegate = self;
      showMessage = nil;
      outputRes = "";
      do
      {
        if (client.connected == true)
        {
          try client.disconnect()
        }
        else
        {
          client.imServer = server
          client.sslEnabled = true
          try client.connectTo(user: username, password: password)
        }
        
        connected = client.connected;
      }
      catch
      {
        do
        {
          try client.disconnect();
        }
        catch {}
        outputRes += "Error: \(error)"
        return
      }
    }, label:
            {
      Text("\(connectedChange())").font(.system(size: 20)).frame(minWidth: 150, minHeight: 40).background(RoundedRectangle(cornerRadius: 8).fill(Color.gray))
    })
    .buttonStyle(PlainButtonStyle())
  }
  
  @ViewBuilder
  private func sendButton() -> some View {
    Button(action:
            {
      do
      {
        client.messageText = message
        var msgIdentifier = try client.sendMessage(jabberId: client.buddies[selectedBuddy].id)
        
        showMessage = ShowMes(message: "Sent Message", title: "");
      }
      catch
      {
        showMessage = ShowMes(message: "Error \(error)")
        return
      }
    }, label: {
      Text("Send Message").font(.system(size: 20)).frame(minWidth: 150, minHeight: 40).background(RoundedRectangle(cornerRadius: 8).fill(Color.gray))
    })
    .buttonStyle(PlainButtonStyle()).disabled(connected == false)
  }
}

struct ContentView_Previews: PreviewProvider {
  static var previews: some View {
    ContentView()
  }
}
