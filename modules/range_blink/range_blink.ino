#include <ESP8266WiFi.h>

const char *ssid =  "Blink-Speed";  
const char *pass =  "12mt61r11";
void setup() {
  Serial.begin(9600);
  delay(100);
  pinMode(LED_BUILTIN, OUTPUT);
   WiFi.begin(ssid, pass); 
       while (WiFi.status() != WL_CONNECTED) 
          {
            delay(500);
            Serial.print(".");
          }
      Serial.println("");
      Serial.println("WiFi connected");
      Serial.print("IP address: ");
      Serial.println(WiFi.localIP());
}
void loop() {
  if(WiFi.status()== WL_CONNECTED) {
    Serial.println("blinking");
     digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
      delay(500);                       // wait for a second
     digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
      delay(500);
  } 
  else{
     digitalWrite(LED_BUILTIN, HIGH);
     Serial.println("solid-blue");}
}
