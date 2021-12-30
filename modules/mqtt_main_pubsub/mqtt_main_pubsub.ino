
//THIS IS FOR BROADCASTING OF MAINS STATE USING PUB-SUB CLIENT LIB

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// Update these with values suitable for your network.
  const char* ssid = "ESP";
  const char* password = "ratcatdog";
  const char* mqtt_server = "192.168.43.64";
  int main;

 WiFiClient espClient;
 PubSubClient client(espClient);

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

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
 }

 void callback(char* topic, byte* payload, unsigned int length) {
    Serial.print("Message arrived [");
    Serial.print(topic);
    Serial.print("] ");
  
    String str;
    for (int i = 0; i < length; i++) {
        str=String(str+(char)payload[i]);
    }
    Serial.println(str);
    if(str.equalsIgnoreCase("mains on")){
        main=1;
    }
    else{
        main=0;
    }
 }

 void reconnect() {
  // Loop until we're reconnected
    while (!client.connected()) {
       Serial.print("Attempting MQTT connection...");
       String clientId = "ESP8266Client-"; // Create a random client ID
       clientId += String(random(0xffff), HEX);
      // Attempt to connect
       if (client.connect(clientId.c_str())) {
         Serial.println("connected");
         client.subscribe("/esp8266/main");  //subscribing to TOPIC "/esp8266/main"
       }
       else {
         Serial.print("failed, rc=");
         Serial.print(client.state());
         Serial.println(" try again in 5 seconds");
       // Wait 5 seconds before retrying
         delay(5000);
      }
   }
 }

 void setup() {
     Serial.begin(9600);
     setup_wifi();
    client.setServer(mqtt_server, 1883);
    client.setCallback(callback);
 }

 void loop() {
    if (!client.connected()) {
       reconnect();
    }
    client.loop();
 }
