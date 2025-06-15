
#include <ESP8266WiFi.h>
const char* ssid = "P";    //  Your Wi-Fi Name
const char* password = "whoareyou";   // Wi-Fi Password
int BUZ_PIN = 2;   // led connected to GPIO2 (D4)
int GAS_PIN = 14;
int waterMotor=15;
int Light=5;

volatile bool gasDetected = false;
WiFiServer server(80);

/*void IRAM_ATTR buttonISR() {
    gasDetected = true;
    digitalWrite(BUZ_PIN,HIGH);
    digitalWrite(waterMotor,HIGH);
    digitalWrite(Light,LOW);
}*/

void setup()
{
  Serial.begin(115200); //Default Baudrate
  pinMode(BUZ_PIN, OUTPUT);
  pinMode(waterMotor, OUTPUT);
  pinMode(GAS_PIN, INPUT_PULLUP);
  pinMode(Light, OUTPUT);
  digitalWrite(BUZ_PIN, LOW);//Initially keeping buzzer OFF
  digitalWrite(waterMotor, LOW);//Initially keeping water motor OFF
  digitalWrite(Light, HIGH);//Initially keeping lights ON
  Serial.print("Connecting to the Newtork..");
  WiFi.begin(ssid, password);
  //attachInterrupt(digitalPinToInterrupt(GAS_PIN), buttonISR, FALLING);

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
  if (client)
  {
      Serial.println("Waiting for new client");
      while(!client.available())
      {
        delay(1);
      }

      String request = client.readStringUntil('\r');
      Serial.println(request);
      client.flush();
      
      if(request.indexOf("/LED_ON") != -1)
      {
        digitalWrite(Light, HIGH); // Turn LED ON
      }

      if(request.indexOf("/LED_OFF") != -1)
      {
        digitalWrite(Light, LOW); // Turn LED OFF
      }

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
        digitalWrite(BUZ_PIN,HIGH);
        digitalWrite(waterMotor,HIGH);
        digitalWrite(Light,LOW);
      }
      else
      {
        client.print("NORMAL");
        digitalWrite(BUZ_PIN,LOW);
        digitalWrite(waterMotor,LOW);
        //digitalWrite(Light,HIGH);
      }
      client.println("<br><br>");
      client.println("<a href=\"/LED_ON\"\"><button>ON</button></a>");
      client.println("<a href=\"/LED_OFF\"\"><button>OFF</button></a><br />");
      client.println("</html>");
      delay(1);
      Serial.println("Client disonnected");
      Serial.println("");
  }
  else
  {
      if(digitalRead(GAS_PIN) == LOW)
      {
        digitalWrite(BUZ_PIN,HIGH);
        digitalWrite(waterMotor,HIGH);
        digitalWrite(Light,LOW);
      }
      else
      {
        digitalWrite(BUZ_PIN,LOW);
        digitalWrite(waterMotor,LOW);
        //digitalWrite(Light,HIGH);
      }
  }
}