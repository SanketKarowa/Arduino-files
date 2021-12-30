#include <NTPClient.h>
#include <WiFiUdp.h>
#include <ESP8266WiFi.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET  LED_BUILTIN  // 4 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const char *ssid     = "Blink-Speed";
const char *password = "12mt61r11";
int d;
const long utcOffsetInSeconds = 19800;
char daysOfTheWeek[7][12] = {"SUNDAY", "MONDAY", "TUESDAY", "WEDNESDAY", "THURSDAY", "FRIDAY", "SATURDAY"};

// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "in.pool.ntp.org", utcOffsetInSeconds);

void setup(){
  Serial.begin(9600);
   // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
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
  display.setCursor (11,0);
  display.setTextSize(2);
  display.print(daysOfTheWeek[timeClient.getDay()]);
  display.setCursor(0,30);
// display.println(timeClient.getFullFormattedTime());
  display.display();
  delay(1000);
}
