


#include <SoftwareSerial.h>
SoftwareSerial ArduinoUno(3,2);

#define RELAY_NO    true
#define NUM_RELAYS  2
int relayGPIOs[NUM_RELAYS] = {8,9};


void setup(){
  
  Serial.begin(9600);
  ArduinoUno.begin(9600);

  for(int i=1; i<=NUM_RELAYS; i++){
      pinMode(relayGPIOs[i-1], OUTPUT);
    if(RELAY_NO){
      digitalWrite(relayGPIOs[i-1], HIGH);
    }
    else{
      digitalWrite(relayGPIOs[i-1], LOW);
  }
 } 
}

void loop(){
  int val;
  if(ArduinoUno.available()>0)
  {
   val = ArduinoUno.parseInt();
   if(ArduinoUno.read()== '$'){
   Serial.println(String("val : ") + val);
  }
}

   if(val==0){  
    digitalWrite(8, LOW);//HIGH for no opto relay
    digitalWrite(9, LOW);//HIGH for no opto relay
    Serial.println("RELAYS ON");
   }
  else if (val==1){
    digitalWrite(8, HIGH);//LOW for no opto relay
    digitalWrite(9, HIGH);//LOW for no opto relay
    Serial.println("RELAYS OFF");
 }

delay(50);
}
