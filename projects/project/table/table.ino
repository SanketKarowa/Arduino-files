void setup() { 
  Serial.begin(9600);
}

void loop() {
  delay(500);
   //------------voltage sensor--------------
  int adc = analogRead(A0);// read the input
  double v;
   Serial.println(String("adc") + adc);
if (adc >= 923 && adc <= 929) 
     v=14.4;
else if(adc >= 917 && adc <=922)
     v =14.3; 
else if(adc >= 910 && adc <=916)
     v =14.2; 
else if(adc >= 903 && adc <=909)
     v =14.1; 
else if(adc >= 898 && adc <=902)
     v =14.0; 
else if(adc >= 891 && adc <=897)
     v =13.9;
else if(adc >= 886 && adc <=890)
     v =13.8 ; 
else if(adc >= 880 && adc <=885)
     v =13.7;
else if(adc >= 873 && adc <=879)
     v =13.6;
else if(adc >= 866 && adc <=872 )
     v =13.5;
else if(adc >= 859 && adc <=865 )
     v =13.4 ;
else if(adc >= 852 && adc <=858 )
     v =13.3;
else if(adc >= 845 && adc <=851 )
     v =13.2;
else if(adc >= 838 && adc <=844)
     v =13.1;
else if(adc >= 831 && adc <=837)
     v =13.0 ;
else if(adc >= 826 && adc <=830)
     v =12.9 ;
else if(adc >= 822 && adc <=825)
     v =12.8 ;
else if(adc >= 815 && adc <=821)
     v =12.7;
else if(adc >= 810 && adc <=814 )
     v =12.6 ;
else if(adc >= 804 && adc <=809 )
     v =12.5  ;
else if(adc >= 796 && adc <=803)
     v =12.4 ;
else if(adc >= 790 && adc <=795)
     v =12.3 ;
else if(adc >= 783 && adc <=789 )
     v =12.2 ;
else if(adc >= 777 && adc <=782)
     v =12.1 ;
else if(adc >= 770 && adc <=776)
     v =12.0 ;
else if(adc >= 763 && adc <=769)
     v =11.9 ;
else if(adc >= 756 && adc <=762)
     v =11.8;
else if(adc >= 749 && adc <=755)
     v =11.7;
else if(adc >= 743 && adc <=750)
     v =11.6 ;
else if(adc >= 737 && adc <=742)
     v =11.5;
else if(adc >= 730 && adc <=736)
     v =11.4;
else if(adc >= 725 && adc <=729)
     v =11.3 ;
else if(adc >= 720 && adc <=724)
     v =11.2;
else if(adc >= 714 && adc <=719)
     v =11.1 ;
else if(adc >= 706 && adc <=713)
     v =11.0 ;
else if(adc >= 698 && adc <=705)
     v =10.9 ;
else if(adc >= 693 && adc <=697)
     v =10.8 ;
else if(adc >= 686 && adc <=692)
     v =10.7  ;
else if(adc >= 679 && adc <=685)
     v =10.6  ;
else if(adc >= 672 && adc <=678)
     v =10.5 ;
else if(adc >= 666 && adc <=671)
     v =10.4  ;
else if(adc > 930){Serial.println("HIGH");}
else if(adc < 930){Serial.println("LOW");}
Serial.println(v);
}      
