#include <ESP8266Ping.h>
#include <ESP8266WiFi.h>

// Set to true to define Relay as Normally Open (NO)
#define RELAY_NO    true
#define NUM_RELAYS  1
// Assign each GPIO to a relay
int relayGPIOs[NUM_RELAYS] = {0};//D3 
//Update these with values suitable for your network.
const char* ssid = "Blink-Speed";
const char* password = "12mt61r11";
const IPAddress remote_ip(192, 168, 43, 1);
WiFiClient espClient;


void setup_wifi() {
 digitalWrite(12,HIGH); //fix
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


void setup() {
    Serial.begin(9600);
 
  for(int i=1; i<=NUM_RELAYS; i++)
   {
       pinMode(relayGPIOs[i-1], OUTPUT);
       if(RELAY_NO){
       digitalWrite(relayGPIOs[i-1], HIGH);
   }
    else{
       digitalWrite(relayGPIOs[i-1], LOW);
  }
 } 
  setup_wifi();
 Serial.println(WiFi.localIP());
}    
 
void loop(){
  delay (50);   
    if(Ping.ping(remote_ip)) {
        //Serial.println("Success!!");
        digitalWrite(0, LOW);//HIGH for no opto relay
        Serial.println("TENDA ON");
      } 
     else {
        //Serial.println("Error :(");
        digitalWrite(0, HIGH);//LOW for no opto relay
        Serial.println("TENDA OFF");
      }
 }

  
