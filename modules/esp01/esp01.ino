
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

//Update these with values suitable for your network.
const char* ssid = "ESP";
const char* password = "ratcatdog";
const char* mqtt_server = "192.168.43.132";
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

  //randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length){
  String str;
  for(int i = 0; i < length; i++){
      str=String(str+(char)payload[i]);
 }
  if(str.equalsIgnoreCase("mains on")){
     main=1; }
  else{
     main=0;}
 
}

void reconnect() {
 //Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESPClient-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())){
      Serial.println("connected");
      client.subscribe("/esp8266/main");
      main=1; //fix for auto relay on after rst
      //Serial.println (main);
      
  } 
    else{
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
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
 
void loop()  {
 
  if (!client.connected()){
      reconnect();
  }
    int i = 1;
  Serial.print(main);
  Serial.println("$");
  
  client.loop();
   delay (30);
}
