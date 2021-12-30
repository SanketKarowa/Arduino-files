#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <LiquidCrystal.h>

LiquidCrystal_I2C lcd(0x3F, 16, 4);
byte solar[8] = //icon for termometer
{
  0b11111,
  0b10101,
  0b11111,
  0b10101,
  0b11111,
  0b10101,
  0b11111,
  0b00000
};

byte battery[8]=
{
  0b01110,
  0b11011,
  0b10001,
  0b10001,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
};

const float VCC   = 5.0;// supply voltage is from 4.5 to 5.5V. Normally 5V.
const int model = 2;   // enter the model number (see below)
float cutOffLimit = 0.2;// set the current which below that value, doesn't matter. Or set 0.5
float sensitivity[] ={
          0.185,// for ACS712ELCTR-05B-T
          0.100,// for ACS712ELCTR-20A-T
          0.066// for ACS712ELCTR-30A-T
     
         }; 


const float QOV =   0.5 * VCC;// set quiescent Output voltage of 0.5V
float vcbatt; // internal variable for voltage
float vcsol;

//offset voltage sensor
int offsetbatt = 0;// set the correction offset value
int offsetsol = 0;// set the correction offset value

void setup() {
     
     Serial.begin(9600);// initialize serial monitor
     lcd.init();
     lcd.backlight(); 
     lcd.createChar(1,solar);
     lcd.createChar(2, battery);
     lcd.clear();
}

void loop() {
  
 
  
  lcd.clear();
 lcd.setCursor(0, 0);
 lcd.print("SOL");
 lcd.setCursor(4, 0);
 lcd.write(1);
 lcd.setCursor(0, 1);
 lcd.print(20);lcd.print("V");
 lcd.print(" ");
 lcd.print(2); lcd.print("A");
 lcd.print(" ");
 lcd.print(40); 
 lcd.begin(0,2);
 lcd.print("BAT");
 lcd.print(" ");
 lcd.write(2);
 lcd.setCursor(0, 1);
 lcd.print(13.8);lcd.print("V");
  lcd.print(" ");
 lcd.print(2); lcd.print("A");
  lcd.print(" ");
 lcd.print(27.6);
  
  delay(2000);
  }
  
