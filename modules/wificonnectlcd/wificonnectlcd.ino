#include <ESP8266WiFi.h>
#include <LiquidCrystal_I2C.h>
#include <LiquidCrystal.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

const char *ssid =  "Hammerhead";     // replace with your wifi ssid and wpa2 key
const char *pass =  "lalalala";
//Serial.println("1");
 
void setup() 
{
       Serial.begin(9600);Serial.println("setup");
       delay(10);
               
  //lcd
       lcd.init();
       lcd.backlight();
       
       Serial.println("Connecting to ");
       Serial.println(ssid); 
       
     lcd.print("Connecting to ");
       lcd.setCursor (0,1);
       lcd.print(ssid);
     
       WiFi.begin(ssid, pass); 
       while (WiFi.status() != WL_CONNECTED) 
          {
            delay(600);
            Serial.print(".");
      lcd.print(".");
          }
      Serial.println("");
      Serial.println("WiFi connected");
      //lcd.print("connected!!");   
}
 
void loop() 
{     
   delay(1000);
   lcd.clear();
    if(WiFi.status()!=3)
    {
    Serial.println("disconnected");
  lcd.print("disconnected!!");
    }
    else
    {
      Serial.println("connected");
      lcd.print("connected!!"); 
  }
    }
