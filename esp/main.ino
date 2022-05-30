#include <ESP8266WiFi.h>
#include <PubSubClient.h>
const char *ssid = "internet_adi";  // bunları doldur
const char *password = "sifre";
const char *mqtt_server = "Raspberry pi ip";

const int bpin = 16;
WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

void setup_wifi()
{

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  
  delay(300);
  
}
void callback(char *topic, byte *message, unsigned int length)
{
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;

  for (int i = 0; i < length; i++)
  {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();
}
void reconnect()
{
  // Loop until we're reconnected
  while (!client.connected())
  {
    
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP8266Client"))
    {
      
      Serial.println("connected");
      
    }
    else
    {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");

      delay(3000);
    }
  }
}
void setup()
{
 
  Serial.begin(9600);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  pinMode(bpin,INPUT);
}
void loop()
{
  if (!client.connected())
  {
    reconnect();
  }
  client.loop();

  long now = millis();
  if (now - lastMsg > 700)
  { 

    lastMsg = now;
    int butval = digitalRead(bpin);
    char butstr[8];
    dtostrf(butval, 1, 2, butstr);
    Serial.print("Işık:");
    Serial.println(butstr);
    client.publish("/btn", butstr);
  }
}
