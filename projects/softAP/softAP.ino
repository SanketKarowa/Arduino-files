#include <ESP8266WiFi.h>

IPAddress local_IP(192,168,4,2);
IPAddress gateway(192,168,4,1);
IPAddress subnet(255,255,255,0);
const char* ssid ="Berry" ;
const char* psk  ="Bloomgum" ;
int channel = 11 ;
int max_connection = 2 ;

void setup()
{
  Serial.begin(9600);
  Serial.println();

  Serial.print("Setting soft-AP configuration ... ");
  Serial.println(WiFi.softAPConfig(local_IP, gateway, subnet) ? "Ready" : "Failed!");

  Serial.print("Setting soft-AP ... ");
 Serial.println(WiFi.softAP(ssid, psk, channel, max_connection) ? "Ready" : "Failed!");


}

void loop() {
  delay(2000);
  Serial.print("Soft-AP IP address = ");
  Serial.println(WiFi.softAPIP());
  WiFi.softAPgetStationNum();
  Serial.printf("Stations connected to soft-AP = %d\n", WiFi.softAPgetStationNum());
  Serial.printf("MAC address = %s\n", WiFi.softAPmacAddress().c_str());
  }
