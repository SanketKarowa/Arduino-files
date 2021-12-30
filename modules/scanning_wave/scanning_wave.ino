#include <LiquidCrystal_I2C.h>
#include <LiquidCrystal.h>

LiquidCrystal_I2C lcd(0x3F, 16, 4);


String m = {"scanning"};
int i;
unsigned int len= m.length();
void setup() {
  lcd.init();
  lcd.backlight();
 // Serial.begin(9600);
}

void loop() {
   lcd.clear();   
   Serial.println(len);
   for (int i=0;i<len;i++) {
        lcd.print("scanning");delay(500);
        lcd.setCursor(i,0);
        lcd.print(m[i]=toUpperCase(m[i]));delay(500);
        lcd.print("scanning");
        lcd.clear(); delay(10);
  }
}
