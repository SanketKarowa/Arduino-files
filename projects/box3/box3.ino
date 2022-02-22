//CODE IN USE FOR BOX2
#include "LiquidCrystal_I2C.h"
#include "Wire.h"
#define DS3231_I2C_ADDRESS 0x68
  
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Convert normal decimal numbers to binary coded decimal
byte decToBcd(byte val){
     return( (val/10*16) + (val%10) );
    }
// Convert binary coded decimal to normal decimal numbers
byte bcdToDec(byte val){
     return( (val/16*10) + (val%16) );
    }
int h;    //hour format correction
float v;  //volts
int adc;  //adc from device

void setup(){
	  Wire.begin();
	  Serial.begin(9600);
	  pinMode(12,OUTPUT);//RELAY NO OPTO where HIGH IS ON
	  pinMode(14,OUTPUT);//USB //mosfet where HIGH IS ON AND VICE VERSA
	  pinMode(16,OUTPUT);//LOAD//mosfet where HIGH IS ON AND VICE VERSA
	  //lcd initializationl
	  lcd.init();
	  lcd.backlight();
	  
	  //------------voltage sensor--------------
	  adc = analogRead(A0);// read the input
	  Serial.println(String("adc") + adc);
    }

void readDS3231time(byte *second,byte *minute,byte *hour,byte *dayOfWeek,byte *dayOfMonth,byte *month,byte *year){
	  Wire.beginTransmission(DS3231_I2C_ADDRESS);
	  Wire.write(0); // set DS3231 register pointer to 00h
	  Wire.endTransmission();
	  Wire.requestFrom(DS3231_I2C_ADDRESS, 7);
	  // request seven bytes of data from DS3231 starting from register 00h
	  *second = bcdToDec(Wire.read() & 0x7f);
	  *minute = bcdToDec(Wire.read());
	  *hour = bcdToDec(Wire.read() & 0x3f);
	  *dayOfWeek = bcdToDec(Wire.read());
	  *dayOfMonth = bcdToDec(Wire.read());
	  *month = bcdToDec(Wire.read());
	  *year = bcdToDec(Wire.read());
    }


void displayTime(){
		byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
		// retrieve data from DS3231
		readDS3231time(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month,
		&year);
		// send it to the serial monitor
		Serial.print(" Day of week: ");
		lcd.clear();
		lcd.setCursor(1,0);
		// DayOfTheWeek
	 switch(dayOfWeek){
		  case 1:
			Serial.print("Sunday");lcd.print("Sun");
			break;
		  case 2:
			Serial.print("Monday");lcd.print("Mon");
			break;
		  case 3:
			Serial.print("Tuesday");lcd.print("Tue");
			break;
		  case 4:
			Serial.print("Wednesday");lcd.print("Wed");
			break;
		  case 5:
			Serial.print("Thursday");lcd.print("Thu");
			break;
		  case 6:
			Serial.print("Friday");lcd.print("Fri");
			break;
		  case 7:
			Serial.print("Saturday");lcd.print("Sat");
			break;
			}
	  //DayOfThemonth
	  Serial.print(" ");
	  Serial.print(dayOfMonth, DEC);
	  lcd.print(" ");
	  lcd.print(dayOfMonth, DEC); 
	  //month
	  switch(month){
		  case 1:
			Serial.print("Jan");lcd.print("Jan");
			break;
		  case 2:
			Serial.print("Feb");lcd.print("Feb");
			break;
		  case 3:
			Serial.print("Mar");lcd.print("Mar");
			break;
		  case 4:
			Serial.print("Apr");lcd.print("Apr");
			break;
		  case 5:
			Serial.print("May");lcd.print("May");
			break;
		  case 6:
			Serial.print("Jun");lcd.print("Jun");
			break;
		  case 7:
			Serial.print("Jul");lcd.print("Jul");
			break;
		  case 8:
			Serial.print("Aug");lcd.print("Aug");
			break;
		  case 9:
			Serial.print("Sep");lcd.print("Sep");
			break;
		  case 10:
			Serial.print("Oct");lcd.print("Oct");
			break;
		  case 11:
			Serial.print("Nov");lcd.print("Nov");
			break;
		  case 12:
			Serial.print("Dec");lcd.print("Dec");
			break;
	  }
	  
	  // year
	  Serial.print(" ");
	  Serial.print(year, DEC);
	  lcd.print(" ");
	  lcd.print(2000+ year);

	  // time clock
	  lcd.setCursor(4,1);
	  Serial.print(" ");
	  Serial.print(hour, DEC);
	 if(hour>12) {
		 h=(hour-12);
		 lcd.print(h);
		 lcd.print(":");
		}
	 else if(hour<10){  
		 lcd.print("0");
		 lcd.print(hour);
		 lcd.print(":");
		}
	 else{  
		 lcd.print(hour);
		 lcd.print(":");
		}
		 
	 // convert the byte variable to a decimal number when displayed
	 Serial.print(":");
	  if(minute<10){
		 Serial.print("0"); lcd.print("0");
	    }
	  Serial.print(minute, DEC);
	  lcd.print(minute, DEC);
	  Serial.print(":");
	  
	  if(second<10){
		 Serial.print("0");
		}
	  Serial.print(second, DEC);
	  Serial.println(" ");
	  lcd.print(" ");
	  if(hour>=12) {
		lcd.print("PM");
		}
	  else {
		 lcd.print("AM");
		}
	  lcd.home();
	  delay(1000);
    }
void loop(){
	  displayTime(); 
	  table();
	  voltage();
	  aux();
	  usb();
	  load();
    }

int table(){
	  int  adc = analogRead(A0);// read the input 
	  Serial.print(String("ADC = ")+adc);

	  if (adc >= 967 && adc <= 944) 
		 v=14.9;
	  else if(adc >= 960 && adc <= 966) 
		 v=14.8;
	  else if(adc >= 952 && adc <= 959) 
		 v=14.7;
	  else if(adc >= 945 && adc <= 951) 
		 v=14.6;
	  else if(adc >= 937 && adc <= 944) 
		 v=14.5;
	  else if(adc >= 930 && adc <= 936) 
		 v = 14.4;
	  else if(adc >= 923 && adc <= 929) 
		 v = 14.3;
	  else if(adc >= 917 && adc <=922)
		 v =14.2; 
	  else if(adc >= 910 && adc <=916)
		 v =14.1; 
	  else if(adc >= 903 && adc <=909)
		 v =14.0;   
	  else if(adc >= 898 && adc <=902)
		 v =13.9; 
	  else if(adc >= 891 && adc <=897)
		 v =13.8;
	  else if(adc >= 886 && adc <=890)
		 v =13.7 ; 
	  else if(adc >= 880 && adc <=885)
		 v =13.6;
	  else if(adc >= 873 && adc <=879)
		 v =13.5;
	  else if(adc >= 866 && adc <=872 )
		 v =13.4;
	  else if(adc >= 859 && adc <=865 )
		 v =13.3 ;
	  else if(adc >= 852 && adc <=858 )
		 v =13.2;
	  else if(adc >= 845 && adc <=851 )
		 v =13.1;
	  else if(adc >= 838 && adc <=844)
		 v =13.0;
	  else if(adc >= 831 && adc <=837)
		 v =12.9 ;
	  else if(adc >= 826 && adc <=830)
		 v =12.8 ;
	  else if(adc >= 822 && adc <=825)
		 v =12.7 ;
	  else if(adc >= 815 && adc <=821)
		 v =12.6;
	  else if(adc >= 810 && adc <=814 )
		 v =12.5 ;
	  else if(adc >= 804 && adc <=809 )
		 v =12.4  ;
	  else if(adc >= 796 && adc <=803)
		 v =12.3 ;
	  else if(adc >= 790 && adc <=795)
		 v =12.2 ;
	  else if(adc >= 783 && adc <=789)
		 v =12.1 ;
	  else if(adc >= 777 && adc <=782)
		 v =12.0 ;
	  else if(adc >= 770 && adc <=776)
		 v =11.9 ;
	  else if(adc >= 763 && adc <=769)
		 v =11.8 ;
	  else if(adc >= 756 && adc <=762)
		 v =11.7;
	  else if(adc >= 749 && adc <=755)
		 v =11.6;
	  else if(adc >= 743 && adc <=750)
		 v =11.5 ;
	  else if(adc >= 737 && adc <=742)
		 v =11.4;
	  else if(adc >= 730 && adc <=736)
		 v =11.3;
	  else if(adc >= 725 && adc <=729)
		 v =11.2 ;
	  else if(adc >= 720 && adc <=724)
		 v =11.1;
	  else if(adc >= 714 && adc <=719)
		 v =11.0 ;
	  else if(adc >= 706 && adc <=713)
		 v =10.9 ;
	  else if(adc >= 698 && adc <=705)
		 v =10.8 ;
	  else if(adc >= 693 && adc <=697)
		 v =10.7 ;
	  else if(adc >= 686 && adc <=692)
		 v =10.6  ;
	  else if(adc >= 679 && adc <=685)
		 v =10.5  ;
	  else if(adc >= 672 && adc <=678)
		 v =10.4 ;
	  else if(adc >= 666 && adc <=671)
		 v =10.3  ;
	  else if(adc >= 661 && adc <=663)
		 v =10.2  ;
	  else if(adc >= 648 && adc <=651)
		 v =10.0  ;
	  else if(adc >= 615 && adc <=618)
		 v =9.5  ;
	  else if(adc >= 603 && adc <=605)
		 v =9.3  ;   
	  else if(adc >= 583 && adc <=585)
		 v =9.0  ;   
	  else if(adc >= 1015 && adc <=1018)
		 v =15.5  ;
		else if( adc <=582)
		 v =0.00  ;      
	  Serial.print(" ");
	  Serial.println(String("volts : ")+ v);
    }

void voltage(){
     lcd.clear();
     lcd.setCursor(0,0);
     lcd.print("volts : ");
     lcd.print(v);
     lcd.print("v");
     delay(1000);
    }
	
void aux(){
     byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
     readDS3231time(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month,&year);
     if((hour >=8) && (hour<=15)){
		 //relay on for aux charging
		 digitalWrite(12, HIGH);
		 lcd.clear();
		 lcd.setCursor(0,0);
		 lcd.print("BATT CHARGING");
		 Serial.println("BATT CHARGING ACTIVE");
		 delay(1000);  
        }
     else {
         digitalWrite(12, LOW);
         Serial.println("BATT CHARGING INACTIVE");
         //delay(1000); 
        }
    }
	
 void usb(){
     byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
     readDS3231time(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month,
     &year);
     
     if((hour>=8) && (hour<=16) && (v >= 11)){
          //relay on for USB charging
          digitalWrite(14, HIGH);// MOSFET ON 
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("USB ACTIVE");
          Serial.println("USB ACTIVE");
          delay(1000);
        }    
		
     else {
		  digitalWrite(14, LOW);//MOSFET OFF
		  lcd.clear();
		  lcd.setCursor(0,0);
		  lcd.print("USB INACTIVE");
		  Serial.println("USB INACTIVE");
		  delay(1000);
        }
    }

 void load(){
     byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
     readDS3231time(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month,
     &year);
    
     if((hour >=17) && (hour <=23) || ((hour >=0) && (hour<=5)) && v >=10.5){
		   //relay on for LOAD
			digitalWrite(16, HIGH);//MOSFET ON 
			lcd.clear();
			lcd.setCursor(0,0);
			lcd.print("LOAD ACTIVE");
			Serial.println("LOAD ACTIVE");
			delay(1000);  
		}
    
	 else {
			digitalWrite(16, LOW); //MOSFET OFF
			lcd.clear();
			lcd.setCursor(0,0);
			lcd.print("LOAD INACTIVE");
			Serial.println("LOAD INACTIVE");
			delay(1000);
        }
    }
