#include <NTPClient.h>
#include <WiFiUdp.h>
#include <ESP8266WiFi.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x3F, 16, 4);

const char *ssid     = "Blink-Speed";
const char *password = "12mt61r11";
int d;
const long utcOffsetInSeconds = 19800;
char daysOfTheWeek[7][12] = {"SUN", "MON", "TUE", "WED", "THU", "FRI", "SAT"};

// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "in.pool.ntp.org", utcOffsetInSeconds);

void setup(){
  Serial.begin(115200);
  //lcd
       lcd.init();
       lcd.backlight();
  WiFi.begin(ssid, password);

  while ( WiFi.status() != WL_CONNECTED ) {
    delay (5); Serial.println(" connecting to WIFI..");
   lcd.print("connecting");
     dot();
  }

  timeClient.begin();
}
void loop() {
  timeClient.update();
  lcd.clear();
    lcd.setCursor (6,1);
  Serial.print(daysOfTheWeek[timeClient.getDay()]);
  lcd.print(daysOfTheWeek[timeClient.getDay()]);
  Serial.print(", ");
  //lcd.print(", ");
  Serial.print(timeClient.getHours());
  lcd.setCursor(0,2);
  lcd.print(timeClient.getHours());
  Serial.print(":");
  lcd.print(":");
  Serial.print(timeClient.getMinutes());
  lcd.print(timeClient.getMinutes());
  Serial.print(":");
  lcd.print(":");
  Serial.println(timeClient.getSeconds());
  lcd.print(timeClient.getSeconds());
  Serial.println(timeClient.getFormattedTime());

  delay(500);
}
int dot(){
  for(d=0;d<=3;d++){
    delay(500);
    lcd.print("."); 
   }
   lcd.clear();
  }
