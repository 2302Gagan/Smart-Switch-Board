#include<WiFi.h>

const char* ssid = "Muskaan";
const char* password = "123muskaan";


WiFiServer server(80);

String header;

String output26State = "off";
String output27State = "off";

const int output26 = 26;
const int output27 = 27;

void setup() {
  Serial.begin(115200);
 
  pinMode(output26, OUTPUT);
  pinMode(output27, OUTPUT);
 
  digitalWrite(output26, LOW);
  digitalWrite(output27, LOW);

  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
 
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

void loop(){
  
  WiFiClient client = server.available();   

  if (client) {                             
    Serial.println("New Client.");          
    String currentLine = "";                
    while (client.connected()) {            
      if (client.available()) {             
        char c = client.read();             
        Serial.write(c);                    
        header += c;
        if (c == '\n') {                    
          if (currentLine.length() == 0) {
            
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            
            if (header.indexOf("GET /26/on") >= 0) {
              Serial.println("Socket 1 on");
              output26State = "on";
              digitalWrite(output26, HIGH);
            } else if (header.indexOf("GET /26/off") >= 0) {
              Serial.println("Socket 1 off");
              output26State = "off";
              digitalWrite(output26, LOW);
            } else if (header.indexOf("GET /27/on") >= 0) {
              Serial.println("Socket 2 on");
              output27State = "on";
              digitalWrite(output27, HIGH);
            } else if (header.indexOf("GET /27/off") >= 0) {
              Serial.println("Socket 2 off");
              output27State = "off";
              digitalWrite(output27, LOW);
            }
           
            
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            client.println("<style>html {font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button {border-radius:25px; background-color: #2ECC71; border: none; color: white; padding: 16px 40px;box-shadow: 0px 2px 18px -4px rgba(0,0,0,0.75);");
            client.println("text-decoration: none; font-size: 23px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {border-radius:25px; background-color: #555555;box-shadow: 0px 2px 18px -4px rgba(0,0,0,0.75);}");
            client.println("div{ background-color:#EAEDED;box-sizing: border-box;box-shadow: 0px 2px 18px -4px rgba(0,0,0,0.75);}</style></head>");
            
           
            client.println("<body><div><h2>Smart Electric Switch Board</h2></div>");
            
            client.println("<p class=\"w\">Socket 1 State " + output26State + "</p>");
                  
            if (output26State=="off") {
              client.println("<p><a href=\"/26/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/26/off\"><button class=\"button button2\">OFF</button></a></p>");
            } 
               
             
            client.println("<p class=\"w\">Socket 2 State " + output27State + "</p>");
                 
            if (output27State=="off") {
              client.println("<p><a href=\"/27/on\"><button class=\"button\">ON</button></a></p><br>");
            } else {
              client.println("<p><a href=\"/27/off\"><button class=\"button button2\">OFF</button></a></p><br>");
            }
            
            client.println("</body></html>");

            
            client.println();
            
            break;
          } else { 
            currentLine = "";
          }
        } else if (c != '\r') {  
          currentLine += c;      
        }
      }
    }
    
    header = "";
    
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}
