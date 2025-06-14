
#include <ESP8266WiFi.h>
const char* ssid = "P";    //  Your Wi-Fi Name
const char* password = "whoareyou";   // Wi-Fi Password
int BUZ_PIN = 2;   // led connected to GPIO2 (D4)
int GAS_PIN = 14;

WiFiServer server(80);

void setup()
{
  Serial.begin(115200); //Default Baudrate
  pinMode(BUZ_PIN, OUTPUT);
  pinMode(GAS_PIN, INPUT);
  digitalWrite(BUZ_PIN, LOW);
  Serial.print("Connecting to the Newtork..");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected"); 
  server.begin();  // Starts the Server
  Serial.println("Server started");
  Serial.print("IP Address of network: "); // will IP address on Serial Monitor
  Serial.println(WiFi.localIP());
  Serial.print("Copy and paste the following URL: http://"); // Will print IP address in URL format
  Serial.print(WiFi.localIP());
  Serial.println("/");
}

void loop()
{
  WiFiClient client = server.available();
  if (!client)
  {
    return;
  }
  Serial.println("Waiting for new client");
  while(!client.available())
  {
    delay(1);
  }

  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();

  request.write();

//*------------------HTML Page Code---------------------*//
  client.println("HTTP/1.1 200 OK"); //
  client.println("Content-Type: text/html");
  client.println("");
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
  client.print(" Gas Leakage Status: ");
  if(digitalRead(GAS_PIN) == LOW)
  {
    client.print("DETECTED");
  }
  else
  {
    client.print("NORMAL");
  }
  client.println("<br><br>");
  client.println("<a href=\"/LED=ON\"\"><button>ON</button></a>");
  client.println("<a href=\"/LED=OFF\"\"><button>OFF</button></a><br />");
  client.println("</html>");
  delay(1);
  Serial.println("Client disonnected");
  Serial.println("");
}