//this dislays the voltage 
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#define SCREEN_WIDTH 128         //OLED display width, in pixels
#define SCREEN_HEIGHT 64         //OLED display height, in pixels
#define OLED_RESET  LED_BUILTIN  // 4 @ Reset pin # (or -1 if sharing Arduino reset pin)

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
String volt;

//Update these with values suitable for your network.
const char* ssid = "";
const char* password = "";
const char* mqtt_server = "";

WiFiClient espClient;
PubSubClient client(espClient);

void setup_wifi() {
	 delay(100);
	 //We start by connecting to a WiFi network
	 Serial.println();
	 Serial.print("Connecting to ");
	 Serial.println(ssid);

	 WiFi.mode(WIFI_STA);
	 WiFi.begin(ssid, password);

	 while(WiFi.status() != WL_CONNECTED) {
		 delay(500);
		 Serial.print("CONNECTING TO "); Serial.println(ssid);
		}

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
        }
 
     if(topic.equals("/esp8266/batt")){
         volt = str ;
         Serial.println(str+String(" v"));
        }
    }  


void reconnect() {
     //Loop until we're reconnected
     while(!client.connected()) {
         Serial.print("Attempting MQTT connection...");
         // Attempt to connect
         if(client.connect("DisplayThing")){
             Serial.print("connected to ");Serial.println(ssid);
             client.subscribe("/esp8266/batt");
            } 
        }
    }

void setup() {
     Serial.begin(9600);
     //OLED
     display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  
     display.display();
     delay(500);
     display.clearDisplay();
    //wifi
     setup_wifi();
     client.setServer(mqtt_server, 1883);
     client.setCallback(callback);
    }    
 
void loop(){
     delay (500);
     if (!client.connected()){
         reconnect();
        } 
     client.loop();
     display.clearDisplay();display.setCursor(3, 16);
     display.setTextSize(3); 
     display.setTextColor(SSD1306_WHITE);
     display.print(volt);display.print("v");
     display.display();
    }  
