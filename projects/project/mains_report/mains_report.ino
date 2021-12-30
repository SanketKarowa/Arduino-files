
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "EmonLib.h"                   // Include Emon Library
EnergyMonitor emon1;                   // Create an instance

const char* mqtt_server = "192.168.43.64";
const char *SSID = "ESP";
const char *PWD = "ratcatdog";
long lastime = 0;
 
void setup_wifi() {
 delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

 Serial.print("Connected - ");
 Serial.println(WiFi.localIP);
}
 
 void callback(char* topic, byte* payload, unsigned int length) {
 Serial.println("Callback");
 Serial.println((char) payload[0]);
}
WiFiClient wifiClient = WiFiClient();
PubSubClient mqttClient(mqttServer, 1883, callback, wifiClient);

void reconnect() {
 Serial.println("Connecting to MQTT Broker...");
 while (!mqttClient.connected()) {
     Serial.println("Reconnecting to MQTT Broker..");
     String clientId = "ESP8266Client-";
     clientId += String(random(0xffff), HEX);
    
     if (mqttClient.connect(clientId.c_str())) {
       Serial.println("Connected.");
       // subscribe to topic      
     }
 }
}
 
void setup() {
 Serial.begin(9600);
setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
    Serial.println(WiFi.localIP());
 
  emon1.current(A0, 111.1);             // Current: input pin, calibration.
}
 
void loop() {
 
 if (!client.connected()){
      reconnect();}
   client.loop();
  
  double Irms = emon1.calcIrms(1480);  // Calculate Irms only
    Serial.println(Irms); 
   if(Irms>10){
    Serial.println("Publishing data..");
    mqttClient.publish("/esp8266/main", "MAINS ON");
    Serial.println("mains on");
   }
  
   else{
     mqttClient.publish("/esp8266/main", "MAINS OFF");
     Serial.println("mains off");
     
    }
  delay(1000);
 }
 