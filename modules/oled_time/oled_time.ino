#include <NTPClient.h>
#include <WiFiUdp.h>
#include <ESP8266WiFi.h>
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
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT,
  OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);

const char *ssid     = "Violet";
const char *password = "Bloomgum";
int d;
const long utcOffsetInSeconds = 19800;
char daysOfTheWeek[7][12] = {"SUNDAY", "MONDAY", "TUESDAY", "WEDNESDAY", "THURSDAY", "FRIDAY", "SATURDAY"};

// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "in.pool.ntp.org", utcOffsetInSeconds);

void setup(){
  Serial.begin(115200);
   if(!display.begin(SSD1306_SWITCHCAPVCC)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 7);
  
  WiFi.begin(ssid, password);

  while ( WiFi.status() != WL_CONNECTED ) {
    delay (5); 
    Serial.println(" connecting to WIFI..");
    display.println("connecting to WIFI"); display.display();display.startscrollright(0x00, 0x0F);
  delay(1000);
  display.stopscroll();
  }
   display.clearDisplay();
   display.setCursor(0,0);
   display.println("Wifi Connected!");display.println();display.display();
   display.println("MY IP ");display.display();
   display.setCursor(0,30);
   display.display();
   display.println(WiFi.localIP() );display.println("\nUpdating time....");display.print("Plz WAIT");
   display.display();
  delay(10000);

  timeClient.begin();
}
void loop() {
  timeClient.update(); Serial.println(timeClient.getFormattedTime());
  display.clearDisplay();
  display.setCursor (0,0);
  display.setTextSize(2);
  display.print(daysOfTheWeek[timeClient.getDay()]);
  display.setCursor(0,30);
  display.println(timeClient.getFullFormattedTime());display.display();
  delay(1000);
}
