#include <ESP8266WiFi.h>
#include <PubSubClient.h>
           

const char* ssid = "ESP";
const char* password = "ratcatdog";
const char* mqtt_server = "192.168.43.64";

String volt_str;
char volt[50];
double dvolt;
int offset = -745;// set the correction offset value

WiFiClient espClient;
PubSubClient client(espClient);


void setup_wifi() {

  delay(10);
 
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
   for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
   Serial.println();
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "d1mini-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
   
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("/esp8266/volt","reconnected");
   
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
   Serial.begin(115200);
   setup_wifi();
   client.setServer(mqtt_server, 1883);
   client.setCallback(callback);
}

void loop() {

  if (!client.connected()) {
    reconnect();
  }
    client.loop();
    int voltin = analogRead(A0);// read the input
    dvolt = map(voltin,0,1023, 0, 2500) + offset;// map 0-1023 to 0-2500 and add correction offset
      dvolt /=100;
      Serial.print(dvolt);
      Serial.println(" V");

      String volt_str=String(dvolt);
      volt_str.toCharArray(volt,volt_str.length() + 1);
      client.publish("/esp8266/volt", volt);
      delay(500);
  }
