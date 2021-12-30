#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Arduino_JSON.h>
#include <ESP8266WiFi.h>
#include <SPI.h>
#include <Wire.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>


#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for SSD1306 display connected using software SPI (default case):
#define OLED_MOSI  14 //d1
#define OLED_CLK   16 //d0
#define OLED_DC    13
#define OLED_CS    15
#define OLED_RESET 12 //res
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT,OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);

// Set to true to define Relay as Normally Open (NO)
#define RELAY_NO    true

// Set number of relays
#define NUM_RELAYS  2

// Assign each GPIO to a relay
int relayGPIOs[NUM_RELAYS] = {4, 5};

// Replace with your network credentials
const char* ssid = "Berry";
const char* password = "Bloomgum";

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <meta charset="utf-8">
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
    input:checked+.slider {background-color: #C0A9BD}
    input:checked+.slider:before {-webkit-transform: translateX(52px); -ms-transform: translateX(52px); transform: translateX(52px)}
  </style>
</head>
<body>
  <h2>Solar Box Server</h2>
  %BUTTONPLACEHOLDER%
<script>
  var url = `ws://${window.location.hostname}/ws`;
  console.log("Initiating websocket connection");
  var socket=new WebSocket(url);
  socket.onopen=function(event){console.log("connection established");}
  socket.onmessage=function(event){
    //update toggle switches
      var reply=event.data;
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
  };
  function toggleCheckbox(element){
    if(element.checked)
    {
      var  request={"action":"update", "relay":element.id, "state":"1"};
    }
    else
    {
      var  request={"action":"update", "relay":element.id, "state":"0"};
    }
    var payload=JSON.stringify(request);
    socket.send(payload);
  }
</script>
</body>
</html>
)rawliteral";

//function to handle requests
void handleWebSocketMessage(void *arg, uint8_t *data, size_t len){
  AwsFrameInfo *info = (AwsFrameInfo*)arg;
  if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT)
  {
    data[len] = 0;
    String request=(char*)data;
    //Serial.println(request);
    JSONVar payload=JSON.parse(request);    
    if (JSON.typeof(payload) == "undefined") {
      Serial.println("Parsing input failed!");
      return;
    }
    if(payload.hasOwnProperty("action"))
    {
      String action=(const char*)payload["action"];
      if(action.equals("update"))
      {
        if(payload.hasOwnProperty("relay") && payload.hasOwnProperty("state"))
        {
          String relayVal=(const char*)payload["relay"];
          String stateVal=(const char*)payload["state"];
          if(RELAY_NO)
            digitalWrite(relayGPIOs[relayVal.toInt()-1], !stateVal.toInt());
          else
            digitalWrite(relayGPIOs[relayVal.toInt()-1], stateVal.toInt());
          //ws.textAll("relay updated");
          char response[5];
          response[0]='1';
          if(digitalRead(4) == 0)
             response[1]='1';
          else response[1]='0';
          response[2]='2';
          if(digitalRead(5) == 0)
             response[3]='1';
          else response[3]='0';
          response[4]='\0';
          ws.textAll(String(response));
        }
      }
    }
  }
}
//websocket specific event handlers
void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type,void *arg, uint8_t *data, size_t len) {
    switch (type) {
      case WS_EVT_CONNECT:
        Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
        break;
      case WS_EVT_DISCONNECT:
        Serial.printf("WebSocket client #%u disconnected\n", client->id());
        break;
      case WS_EVT_DATA:
        handleWebSocketMessage(arg, data, len);
        break;
      case WS_EVT_PONG:
      case WS_EVT_ERROR:
        break;
  }
}
//initialize the websocket server
void initWebSocket() {
  ws.onEvent(onEvent);
  server.addHandler(&ws);
}

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
void setup_wifi() {
 delay(1000);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  digitalWrite(4,HIGH); // non opto
  digitalWrite(5,HIGH); // non opto
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(2000);
   
     //oled
        display.clearDisplay();delay(2000);
        display.setTextSize(1.7); // Draw 2X-scale text
        display.setTextColor(SSD1306_WHITE);
        display.setCursor(2, 0);
        display.println("HELLO SONY PINGUA,");
        display.setCursor(2, 16);
        display.println("TURN ON MOBILE WIFI");
        display.setCursor(4, 26);
        display.println("TO INTERACT");
        display.display();      // Show initial text
       // delay(2000);
  }
 
        Serial.println("");
        Serial.println("WiFi connected");
        Serial.println("IP address: ");
        Serial.println(WiFi.localIP());

   //oled
      display.clearDisplay();
      display.setTextSize(1.5); // Draw 2X-scale text
      display.setTextColor(SSD1306_WHITE);
      display.setCursor(0, 0);
      display.print(F("IP : "));
      display.print(WiFi.localIP());
      display.setCursor(0, 16);
      display.print(F("CONNECTED ,"));//delay(500);
      display.setCursor(2, 32);
      display.println(F("PLEASE WAIT CHECKING,"));
      display.setCursor(2, 42);
      display.println(F("RELAY STATUS."));
      display.display();
}

void setup(){
  // Serial port for debugging purposes
  Serial.begin(9600);

   // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
      if(!display.begin(SSD1306_SWITCHCAPVCC)) {
        Serial.println(F("SSD1306 allocation failed"));
        for(;;); // Don't proceed, loop forever
  }
    // display.display();
      
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
  setup_wifi();
     
  //start the websocket server
  initWebSocket();

  // Print ESP8266 Local IP Address
  Serial.println(WiFi.localIP());


  
  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor);
  });

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor);
  });

  // Start server
  server.begin();

}

void loop() {
  ws.cleanupClients();
  if(WiFi.status() != WL_CONNECTED){setup_wifi();}

    //oled local-ip 
      display.clearDisplay();
      display.setTextSize(1.5); // Draw 2X-scale text
      display.setTextColor(SSD1306_WHITE);
      display.setCursor(0, 0);
      display.print(F("IP : "));
      display.print(WiFi.localIP());
      display.setCursor(2, 16);
      display.print(F("Solar Box Server")); 
      display.setCursor(5, 32);
      display.print("RELAY #4 : ");
        if((digitalRead(4))==1){display.print("OFF");}
        else { display.print("ON"); };

      display.setCursor(5, 42);
      display.print("RELAY #5 : ");
        if((digitalRead(5))==1){display.print("OFF");}
        else { display.print("ON"); };
        
        display.display();
          delay(500);
  }
