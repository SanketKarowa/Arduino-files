#include <SoftwareSerial.h>
SoftwareSerial nano(3,2);

void setup(){
	
	Serial.begin(9600);
	nano.begin(9600);

}

void loop(){
	
	while(nano.available()>0){
	int val = nano.parseInt();
	if(nano.read()== '$'){
	Serial.println(val);
	}
}
delay(30);
}
