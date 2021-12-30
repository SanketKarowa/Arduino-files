/*********
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp8266-relay-module-ac-web-server/
  
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*********/

// Import required libraries
#include "ESP8266WiFi.h"
#include "ESPAsyncWebServer.h"
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for SSD1306 display connected using software SPI (default case):
#define OLED_MOSI  13
#define OLED_CLK   14
#define OLED_DC    4
#define OLED_CS    15
#define OLED_RESET 5

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT,OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS)
// Set to true to define Relay as Normally Open (NO)
#define RELAY_NO    true
#define NUM_RELAYS  2
;
// Assign each GPIO to a relay
int relayGPIOs[NUM_RELAYS] = {2,12};


// Replace with your network credentials
const char* ssid = "Violet";
const char* password = "lalalala";

const char* PARAM_INPUT_1 = "relay";  
const char* PARAM_INPUT_2 = "state";

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
    html {font-family: Arial; display: inline-block; text-align: center;}
    h2 {font-size: 3.0rem;}
    p {font-size: 3.0rem;}
    body {max-width: 600px; margin:0px auto; padding-bottom: 25px;}
    .switch {position: relative; display: inline-block; width: 120px; height: 68px} 
    .switch input {display: none}
    .slider {position: absolute; top: 0; left: 0; right: 0; bottom: 0; background-color: #ccc; border-radius: 34px}
    .slider:before {position: absolute; content: ""; height: 52px; width: 52px; left: 8px; bottom: 8px; background-color: #fff; -webkit-transition: .4s; transition: .4s; border-radius: 68px}
    input:checked+.slider {background-color: #2196F3}
    input:checked+.slider:before {-webkit-transform: translateX(52px); -ms-transform: translateX(52px); transform: translateX(52px)}
  </style>
</head>
<body onload="refresh();">
  <h2>ESP Web Server</h2>
  %BUTTONPLACEHOLDER%
<script>
function toggleCheckbox(element) {
  var xhr = new XMLHttpRequest();
  if(element.checked){ xhr.open("GET", "/update?relay="+element.id+"&state=1", true); }
  else { xhr.open("GET", "/update?relay="+element.id+"&state=0", true); }
  xhr.send();
}
function refresh(){
  var xhr=new XMLHttpRequest();
  xhr.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      var reply=this.responseText;
      var relay1state=reply.slice(1,2);
      var relay2state=reply.slice(3);
      var relay1=document.getElementById('1');
      var relay2=document.getElementById('2');
      if(relay1state == 0)
       relay1.checked=false;
      else relay1.checked=true;
      if(relay2state == 0)
       relay2.checked=false;
      else relay2.checked=true;
      //console.log('relay 1 : '+relay1state+' relay 2 : '+relay2state);
    }
  };
  xhr.open("GET", "/refresh", true);
  xhr.send();
  }
  setInterval(refresh,1000);
 </script>
   </body>
   </html>
)rawliteral";

// Replaces placeholder with button section in your web page
String processor(const String& var){
  //Serial.println(var);
  if(var == "BUTTONPLACEHOLDER"){
    String buttons ="";
    for(int i=1; i<=NUM_RELAYS; i++){
      String relayStateValue = relayState(i);
      buttons+= "<h4>Relay #" + String(i) + " - GPIO " + relayGPIOs[i-1] + "</h4><label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"" + String(i) + "\" "+ relayStateValue +"><span class=\"slider\"></span></label>";
    }
    return buttons;
  }
  return String();
}

String relayState(int numRelay){
  if(RELAY_NO){
    if(digitalRead(relayGPIOs[numRelay-1])){
      return "";
    }
    else {
      return "checked";
    }
  }
  else {
    if(digitalRead(relayGPIOs[numRelay-1])){
      return "checked";
    }
    else {
      return "";
    }
  }
  return "";
}

void setup(){
   // Serial port for debugging purposes
  Serial.begin(9600);
     if(!display.begin(SSD1306_SWITCHCAPVCC)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  
  // Set all relays to off when the program starts - if set to Normally Open (NO), the relay is off when you set the relay to HIGH
  for(int i=1; i<=NUM_RELAYS; i++){
    pinMode(relayGPIOs[i-1], OUTPUT);
    if(RELAY_NO){
      digitalWrite(relayGPIOs[i-1], HIGH);
    }
    else{
      digitalWrite(relayGPIOs[i-1], LOW);
    }
  }
  
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
       delay(1000);
    Serial.println("Connecting to WiFi..");
  display.println("Connecting");    
  display.display();

  // Print ESP8266 Local IP Address
  Serial.println(WiFi.localIP());
 // display.print(WiFi.localIP()); 
  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor);
  });

  // Send a GET request to <ESP_IP>/update?relay=<inputMessage>&state=<inputMessage2>
  server.on("/update", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String inputMessage;
    String inputParam;
    String inputMessage2;
    String inputParam2;
    // GET input1 value on <ESP_IP>/update?relay=<inputMessage>
    if (request->hasParam(PARAM_INPUT_1) & request->hasParam(PARAM_INPUT_2)) {
      inputMessage = request->getParam(PARAM_INPUT_1)->value();
      inputParam = PARAM_INPUT_1;
      inputMessage2 = request->getParam(PARAM_INPUT_2)->value();
      inputParam2 = PARAM_INPUT_2;
      if(RELAY_NO){
        Serial.print("NO ");
        digitalWrite(relayGPIOs[inputMessage.toInt()-1], !inputMessage2.toInt());
      }
      else{
        Serial.print("NC ");
        digitalWrite(relayGPIOs[inputMessage.toInt()-1], inputMessage2.toInt());
      }
    }
    else {
      inputMessage = "No message sent";
      inputParam = "none";
    }
    Serial.println(inputMessage + inputMessage2);
    request->send(200, "text/plain", "OK");

  });
  //refresh the page
  server.on("/refresh", HTTP_GET, [] (AsyncWebServerRequest *request) {
  char response[5];
  response[0]='1';//relaynum
  if((digitalRead(2)==0))//readstate
     response[1]='1';
    
  else response[1]='0';
      response[2]='2';//relaynum
  
  if((digitalRead(12)==0))//readstate
      response[3]='1';
  
  else response[3]='0';
       response[4]='\0';
       request->send(200, "text/plain", response);
  });

  // Start server
  server.begin();
}
}  
void loop() {
    
  delay(100);
   
    if(WiFi.status()!=3)
    {
       display.clearDisplay();
       display.setTextSize(1);display.setCursor(0,0);
       display.println("Hey There!!!"); 
       display.setCursor(0,20);
       display.print("I am Reconnecting !!");
       display.display();
    }
    else{
       display.clearDisplay();
       display.setTextSize(1);
       display.setCursor(0,0);
       display.print("MY IP: ");
       display.print( WiFi.localIP()); 
       delay(1000);
       display.setCursor(0,40);
       display.setTextSize(2);
        
     if((digitalRead(2)==0))
          display.print("ON ");
     else 
         display.print("OFF");
         display.setCursor(90,40);
     if((digitalRead(12)==0))
         display.print("ON ");
     else 
         display.print("OFF");
         display.display();
  }
  
}
