//sameer edlabadkar
// firstly you have to install the board ESP8266 on your IDE
// after puttin SSId and Password of your router 
// you can use the ESP for home automation.

#include <ESP8266WiFi.h>
#include <WiFiManager.h> 
// Replace with your network credentials
const char* ssid     = "";  //WIFI Name
const char* password = "";  //WIFI Password

// Set web server port number to 80
WiFiServer server(80);

// Variable to store the HTTP request
String header;

// Auxiliar variables to store the current output state
String Relay1State = "off";
String Relay2State = "off";
String Relay3State = "off";
String Relay4State = "off";

// Assign output variables to Relay pins
const int Relay1 = 12; //D6
const int Relay2 = 16; //D0
const int Relay3 = 5;  //D1
const int Relay4 = 4;  //D2

// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0; 
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;

void setup() {
  WiFiManager wifiManager;
  wifiManager.autoConnect("sameerEdlabadkar", "12345");
  wifiManager.autoConnect("sameerEdlabadkar");
  Serial.begin(115200);
  // Initialize the output variables as outputs
  pinMode(Relay1, OUTPUT);
  pinMode(Relay2, OUTPUT);
  pinMode(Relay3, OUTPUT);
  pinMode(Relay4, OUTPUT);
  // Set outputs to LOW
  digitalWrite(Relay1, LOW);
  digitalWrite(Relay2, LOW);
  digitalWrite(Relay3, LOW);
  digitalWrite(Relay4, LOW);

  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

void loop(){
  WiFiClient client = server.available();   // Listen for incoming clients

  if (client) {                             // If a new client connects,
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    currentTime = millis();
    previousTime = currentTime;
    while (client.connected() && currentTime - previousTime <= timeoutTime) { // loop while the client's connected
      currentTime = millis();         
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            
            // turns the Relays on and off
            if (header.indexOf("GET /5/on") >= 0) {
              Serial.println("Relay 5 on");
              Relay1State = "on";
              digitalWrite(Relay1, HIGH);
            } else if (header.indexOf("GET /5/off") >= 0) {
              Serial.println("Relay 5 off");
              Relay1State = "off";
              digitalWrite(Relay1, LOW);
            } else if (header.indexOf("GET /4/on") >= 0) {
              Serial.println("Relay 4 on");
              Relay2State = "on";
              digitalWrite(Relay2, HIGH);
            } else if (header.indexOf("GET /4/off") >= 0) {
              Serial.println("Relay 4 off");
              Relay2State = "off";
              digitalWrite(Relay2, LOW);
            }
            else if (header.indexOf("GET /0/on") >= 0) {
              Serial.println("Relay 0 off");
              Relay3State = "on";
              digitalWrite(Relay3, HIGH);
            }
            else if (header.indexOf("GET /0/off") >= 0) {
              Serial.println("Relay 0 off");
              Relay3State = "off";
              digitalWrite(Relay3, LOW);
            }
            else if (header.indexOf("GET /2/on") >= 0) {
              Serial.println("Relay 2 off");
              Relay4State = "on";
              digitalWrite(Relay4, HIGH);
            }
            else if (header.indexOf("GET /2/off") >= 0) {
              Serial.println("Relay 2 off");
              Relay4State = "off";
              digitalWrite(Relay4, LOW);
            }
            
            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // CSS to style the on/off buttons 
            // Feel free to change the background-color and font-size attributes to fit your preferences
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #52527a; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #d1d1e0;}</style></head>");
            
            // Web Page Heading
            client.println("<body><h1>Home Automation SameerEdlabadkar</h1>");
            
            // Display current state, and ON/OFF buttons for Relay 5  
            client.println("<p>Relay 1 - State " + Relay1State + "</p>");
            // If the Relay1State is off, it displays the ON button       
            if (Relay1State=="off") {
              client.println("<p><a href=\"/5/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/5/off\"><button class=\"button button2\">OFF</button></a></p>");
            } 
               
            // Display current state, and ON/OFF buttons for Relay 4  
            client.println("<p>Relay 2 - State " + Relay2State + "</p>");
            // If the Relay2State is off, it displays the ON button       
            if (Relay2State=="off") {
              client.println("<p><a href=\"/4/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/4/off\"><button class=\"button button2\">OFF</button></a></p>");
            }

            // Display current state, and ON/OFF buttons for Relay 0  
            client.println("<p>Relay 3 - State " + Relay3State + "</p>");
            // If the Relay3State is off, it displays the ON button       
            if (Relay3State=="off") {
              client.println("<p><a href=\"/0/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/0/off\"><button class=\"button button2\">OFF</button></a></p>");
            }

            // Display current state, and ON/OFF buttons for Relay 4  
            client.println("<p>Relay 4 - State " + Relay4State + "</p>");
            // If the Relay4State is off, it displays the ON button       
            if (Relay4State=="off") {
              client.println("<p><a href=\"/2/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/2/off\"><button class=\"button button2\">OFF</button></a></p>");
            }
            client.println("</body></html>");
            
            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}
