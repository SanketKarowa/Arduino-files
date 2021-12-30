#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <SPI.h>
#include <Wire.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels


// Declaration for SSD1306 display connected using software SPI (default case):
#define OLED_MOSI  14 //d1
#define OLED_CLK   16 //d0
#define OLED_DC    13
#define OLED_CS    15
#define OLED_RESET 12 //res
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT,OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);

// Set to true to define Relay as Normally Open (NO)
#define RELAY_NO    true
#define NUM_RELAYS  1
// Assign each GPIO to a relay
int relayGPIOs[NUM_RELAYS] = {2};

//Update these with values suitable for your network.
const char* ssid = "Blink-Speed";
const char* password = "12mt61r11";
const char* mqtt_server = "192.168.43.1";
  bool manual_mode=false;
  int    main;
  int    adc ;
  double v; //holds the volt 
  double offset =0;
  char   d[5];
  
WiFiClient espClient;
PubSubClient client(espClient);

void setup_wifi() {
 delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  digitalWrite(2,LOW);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(2000);
    Serial.print(".");
    
  //oled  
  display.clearDisplay();
  display.setTextSize(1); // Draw 2X-scale text
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(10, 0);
  display.println(F("connecting"));
  display.display();      // Show initial text
  display.startscrollleft(0x00, 0x0F);
  }
  display.stopscroll();
  
  //randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  display.clearDisplay();
  display.setTextSize(1.5); // Draw 2X-scale text
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(10, 20);
  display.println(F("connected"));//delay(500);
   //display.display();
  display.setCursor(12, 7);
  display.setTextSize(1); // Draw 2X-scale tex
  display.print(WiFi.localIP());display.display();
}

void callback(String topic, byte* payload, unsigned int length){
   Serial.print("Message arrived [");
   Serial.print(topic);
   Serial.print("] ");
  
  String str;
   for(int i = 0; i < length; i++){
      str=String(str+(char)payload[i]);
      }
      //Serial.println(str);
 
  if(topic.equals("/esp8266/main")  && manual_mode==false){
  if(str.equalsIgnoreCase("mains on")){
    Serial.println(" mains on ");
     main=1;
   }
 
  else{
    Serial.println(" mains off");
     main=0;
   }
   manual_mode=false;
  }
  
 if(manual_mode==true || topic.equals("/esp8266/man/r1"))
 {  manual_mode=true;
   if (topic.equals("/esp8266/man/r1") && str.equals("10")){
    digitalWrite(2, HIGH);//HIGH for no opto relay
    //digitalWrite(12, LOW);//HIGH for no opto relay
    Serial.println("Manual mode - mains off");
    main=0;
   }
  if (topic.equals("/esp8266/man/r1") && str.equals("11")){
    digitalWrite(2, LOW);//LOW for no opto relay
   // digitalWrite(12, HIGH);//LOW for no opto relay
    Serial.println("Manual mode - mains on");
    main=1;  manual_mode=false;
  }
 // manual_mode=true;
 }
} 

void reconnect() {
 //Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    display.clearDisplay();display.setCursor(10, 1);
    display.setTextSize(1.5); // Draw 2X-scale tex
    display.print("Connecting MQTT"); display.display();
    
    //Create a random client ID
    //String clientId = "ESP-Relaybox";
    //clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect("ESP-Relaybox")){
      Serial.println("connected");
      client.subscribe("/esp8266/main"); client.subscribe("/esp8266/man/r1");
      main=1; //fix for auto relay on after rst
      Serial.println (main);  
  } 
    else{
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
  }
 }
}

void setup() {
  Serial.begin(9600);
 // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
 // Clear the buffer
 // display.clearDisplay();
  
  for (int i=1; i<=NUM_RELAYS; i++) {
     pinMode(relayGPIOs[i-1], OUTPUT);
     if (RELAY_NO){
       digitalWrite(relayGPIOs[i-1], HIGH);
     }
     else {
         digitalWrite(relayGPIOs[i-1], LOW);
     }
  } 
  setup_wifi();
    client.setServer(mqtt_server, 1883);
    client.setCallback(callback);
    //Print ESP8266 Local IP Address
    //Serial.println(WiFi.localIP());
}    


 int table(){
 adc = analogRead(A0);// read the input
 // double v; //holds the volt 
  Serial.println(String("ADC ")+adc);

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
  else if(adc >= 783 && adc <=789 )
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

   v=v+offset;  
   Serial.print(String("volts : ")+ v);
   Serial.print(String(" ------- "));

    display.clearDisplay();display.setCursor(1, 16);
    display.setTextSize(1); // Draw 2X-scale tex
    display.print("VOLTS : ");display.print(v);display.println("v");
    display.setCursor(1, 28);
    display.print("RELAY : ");if((digitalRead(2))==1){display.print("ON");}else {display.print("OFF"); };
    display.setCursor(1, 1);
    display.print("IP : ");display.print(WiFi.localIP());display.display();
    display.display();
  } //table end

void loop(){
    delay (500);
    if (!client.connected()){
       reconnect();
     }
    client.loop();
    table();     
    String v_str= String(v);
    v_str.toCharArray(d, v_str.length() + 1);
    client.publish("/esp8266/batt", d);
   
     if(main ==0 && v > 10.9){  //(voltage > 10.9)
        digitalWrite(2,HIGH);//HIGH for no opto relay
        //digitalWrite(10, LOW);//HIGH for no opto relay
        Serial.print("RELAYS ON"); Serial.print(String(" ------- "));
     }
     
    else if (main==1){
      digitalWrite(2, LOW);//LOW for no opto relay
     // digitalWrite(10, HIGH);//LOW for no opto relay
      Serial.print("RELAYS OFF");
        Serial.print(String(" ------- "));
     }
 
    //------status relay-------
    if((digitalRead(2))==1)
        client.publish("/status/r1","11");
    else        
        client.publish("/status/r1","10");

    //------QUICK SAND------
 
    if(v <= 10.8){
     for( ; ; ) {
   
       table();
       digitalWrite(2, LOW);//LOW for no opto relay
       //digitalWrite(10, HIGH);//LOW for no opto relay
       Serial.print("QS");
       Serial.print(String(" ------- "));
       
       client.publish("/esp8266/batt", "QS");

        display.clearDisplay();display.setCursor(1, 16);
        display.setTextSize(1); // Draw 2X-scale tex
        display.print("VOLTS : ");display.print(v);display.println("v");
        display.setCursor(1, 28);
        display.print("RELAY : ");
        
        if ((digitalRead(2))==1) {
          display.print("ON");
          client.publish("/status/r1","11");
         } 
        else {
            display.print("OFF");
            client.publish("/status/r1","10");
        }  
        
        //display.display();
        display.setCursor(1, 46);
        display.setTextSize(1); 
        display.print("QS MODE!! ");

         display.setCursor(1, 1);
         display.print("IP : ");display.print(WiFi.localIP());
         display.display();delay(1000);
       //break statement 
      if(v >= 13.8) {
       Serial.println("ESC");
       client.publish("/esp8266/batt", "ESC");

        display.clearDisplay();display.setCursor(1, 16);
        display.setTextSize(1); // Draw 2X-scale tex
        display.print("VOLTS : ");display.print(v);display.println("v");
        display.setCursor(1, 28);
        display.print("RELAY : ");
        if((digitalRead(2))==1){display.print("ON");}else { display.print("OFF"); };
        //display.display();
        display.setCursor(1, 46);
        display.setTextSize(1); // Draw 2X-scale tex
        display.print("ESCAPE!! "); 
        display.setCursor(1, 1);
        display.print("IP : "); display.print(WiFi.localIP());
        display.display();
        
        delay(500);
       break;
    }
   }
  }
  
 }
