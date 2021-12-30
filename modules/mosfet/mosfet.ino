
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
// Set to true to define Relay as Normally Open (NO)
#define RELAY_NO    true
#define NUM_RELAYS  1
// Assign each GPIO to a relay
int relayGPIOs[NUM_RELAYS] = {0};

//Update these with values suitable for your network.
const char* ssid = "Blink-Speed";
const char* password = "12mt61r11";
const char* mqtt_server = "192.168.43.1";
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

   // digitalWrite(0,LOW);
   // digitalWrite(5,LOW);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print("CONNECTING TO "); Serial.println(ssid);
  }

  //randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(String topic, byte* payload, unsigned int length) {
   Serial.print("Message arrived [");
   Serial.print(topic);
   Serial.print("] ");
  
  String str;
   for(int i = 0; i < length; i++){
      str=String(str+(char)payload[i]);
      }//Serial.println(str);
 
  if (topic.equals("/esp8266/main")) {
     if (str.equalsIgnoreCase("mains on")){
        Serial.println(" mains on ");
        main=1;
     }
 
  else {
       Serial.println(" mains off");
       main=0;
  }
 }
}  


void reconnect() {
 //Loop until we're reconnected
  while (!client.connected()) {
       Serial.print("Attempting MQTT connection...");
       // Attempt to connect
       if (client.connect("MOS-Box")){
          Serial.print("connected to ");Serial.println(ssid);
          client.subscribe("/esp8266/main");
          main=1; //fix
          Serial.println (main);
        } 
    else  {
         Serial.print("failed, rc=");
         Serial.print(client.state());
         Serial.println(" try again in 5 seconds");
         delay(5000);
     }
   }
 }

void setup() {
  Serial.begin(9600);
  Serial.println("setup");
  for(int i=1; i<=NUM_RELAYS; i++){
    pinMode(relayGPIOs[i-1], OUTPUT);
    if(RELAY_NO){
      digitalWrite(relayGPIOs[i-1], HIGH);
  }
    else{
      digitalWrite(relayGPIOs[i-1], LOW);
  }
 } 
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  //Print ESP8266 Local IP Address
    Serial.println(WiFi.localIP());
}    
 
void loop(){
  delay (1000);
  if (!client.connected()){
    reconnect();
 }
  client.loop();
  if(main==0){
    digitalWrite(0, HIGH);//HIGH for no opto relay
    Serial.println("MOSFET ON");
 }
  else if (main==1){
    digitalWrite(0, LOW);//LOW for no opto relay
    //digitalWrite(5, LOW);//LOW for no opto relay
    Serial.println("MOSFET OFF");
   
 }
      /*------status relay-------
       if((digitalRead(0))==1)
       client.publish("/status/r2","21");
       else        
       client.publish("/status/r2","20");
      */
}
