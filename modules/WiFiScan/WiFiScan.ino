
#include "ESP8266WiFi.h"
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);
void setup() {
  Serial.begin(115200);


 //lcd
       lcd.init();
       lcd.backlight();
	   
  // Set WiFi to station mode and disconnect from an AP if it was previously connected
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  Serial.println("Setup done");
}

void loop() {
  Serial.println("scan start");
  lcd.clear();
  lcd.print("scanning");
  
  // WiFi.scanNetworks will return the number of networks found
  int n = WiFi.scanNetworks();
  Serial.println("scan done");
   lcd.clear();
   lcd.print("done");
  if (n == 0) {
    Serial.println("no networks found");
     lcd.clear();	 
	 lcd.print("0network");
  } else {
    Serial.print(n);
    Serial.println(" networks found");
	lcd.clear();
	lcd.print(n);
	lcd.print("network");
	
    for (int i = 0; i < n; ++i) {
      // Print SSID and RSSI for each network found
      Serial.print(i + 1);
	  lcd.home();
	  lcd.print(i + 1);
      Serial.print(": ");
	  lcd.print(": ");
      Serial.print(WiFi.SSID(i));
	  lcd.print(WiFi.SSID(i));
      Serial.print(" (");
	 // lcd.print(" (");
      Serial.print(WiFi.RSSI(i));
	 // lcd.print(WiFi.RSSI(i));
      Serial.print(")");
	 //  lcd.print(" )");
      Serial.println((WiFi.encryptionType(i) == ENC_TYPE_NONE) ? " " : "*");
	 // lcd.print((WiFi.encryptionType(i) == ENC_TYPE_NONE) ? " " : "*");
	  
      delay(1000);
    }
  }
  Serial.println("");
  lcd.print("");

  // Wait a bit before scanning again
  delay(5000);
}
