//Parameters
//Working version TEST 10 feb 22:21
//
const int aisPin  = A4; //a0-mini.... a4-nano

const int numReadings  = 60;
int readings [numReadings];
int readIndex  = 0;
long total  = 0;
//int limitWish = 0; 

long blinkcounter_hour = 0;
int minutes_into = 0;
int watt = 0;
int over5kw_accu = 0; //when consumtion is over 5kW (KWh_warning) NB! int gives max 32767 i.e. 32kwh in one hour maximum
int pause_peak_accu = 0;

int accu_limit = 1000; //if over5kw_accu > accu_limit.  
int KWh_limit = 4580;//5000-420; //Wh //safty margin. Goal is max. 5 kwH / hour
float KWh_limit_f = 4.9; //Kwh //safty margin. Goal is max. 5 kwH / hour
int KWh_warning = 5000; //Wh
float kw_per_pulse = 0.001;
float predict_hour_by_average = 0;
boolean flag_count = 1;
const byte blue = 3; //blueLED
const byte red = 5; //redLED
const byte green = 4; //greenLED
const byte cutoff_relay = 6; //rele NC normally closed Now also with NO

// long day = 86400000; // 86400000 milliseconds in a day
 long hour = 3600000; // 3600000 milliseconds in an hour
 long minute = 60000; // 60000 milliseconds in a minute
 //long second =  1000; // 1000 milliseconds in a second
long refresh_time;
long refresh_time_5;
long refresh_time_hour;
long tid_watt = 0;
 
const long Count_result_after_this_time_passed = minute*5; //5 minutes 
const long Count_result_after_one_hour_passed = hour; //60000= 1 min 

int measured_value  = 0;

void setup() {

  Serial.begin(115200);
  Serial.println(F("Initialize System"));

  pinMode(aisPin, INPUT);
  pinMode(blue, OUTPUT);
  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(cutoff_relay, OUTPUT);

  digitalWrite(blue,HIGH); //HIGH = LED off
  digitalWrite(red,HIGH);
  digitalWrite(green,HIGH);

}

void loop() {

if (millis() > refresh_time_hour) {  //if an hour passed
refresh_time_hour = millis() + Count_result_after_one_hour_passed;
  Serial.print(F("Kwh used this hour: "));
  Serial.println(kw_per_pulse*blinkcounter_hour);
minutes_into=0;  //counts minutes. Reset every whole hour
blinkcounter_hour = 0; //number of "0,001Wh" reset every hour
over5kw_accu = 0; //reset every hour
pause_peak_accu = 0;

}   //refresh_time_hour  


if (millis() > refresh_time) {  //adds minutes
refresh_time = millis() + minute;
Serial.print("predict_hour_by_average minute: "); //
Serial.println(kw_per_pulse*blinkcounter_hour/minutes_into*60);
minutes_into++;
}

if (millis() > (refresh_time_5)) {  //5 minutes//only make the relay change state every 5 minutes
refresh_time_5 = millis() + Count_result_after_this_time_passed;

 Serial.println (" ");
 Serial.print(F("kwh brukt siste "));
 Serial.print(minutes_into);

 Serial.print(F(" minutt: "));
 Serial.print(kw_per_pulse*blinkcounter_hour);
 Serial.println(F(" kWh"));

  //average 
  Serial.println (" ");  
  Serial.print(F("predict_hour_by_average: "));
predict_hour_by_average = kw_per_pulse*blinkcounter_hour/(minutes_into-1)*60;

  Serial.print (predict_hour_by_average);
  Serial.println (F(" kWh")); 

}   //refresh_time
 


if (readAnalogSensor()==1) { //pulse from e-meter detected.
 
      watt = 3600000/(millis()-tid_watt); //power on this pulse 
      blinkcounter_hour++; // number of pulses so far (watthours) - in this hour 
      

    //#1 Green_state

      if (watt<KWh_warning) {// && blinkcounter_hour<KWh_limit) {
      digitalWrite(green,LOW); //LOW = LED on
      //pause_peak_accu++;
      }

    //#2 Red_state
      if (watt>=KWh_warning) 
      digitalWrite(red,LOW); //LOW = LED on




//#3 Blue_state
if (blinkcounter_hour>=(KWh_limit) || predict_hour_by_average>=KWh_limit_f) {
  digitalWrite(blue,LOW); //LOW = LED on
  digitalWrite(cutoff_relay,HIGH); //energizing coil on NC relay -> cut power to contactors on boiler and waterheater
}
else {
  digitalWrite(cutoff_relay,LOW);
}



     
      tid_watt = millis();
      watt= 0;
      } // (readAnalogSensor()==1)



else {

    delay(5); //led a bit brighter
    digitalWrite(green,HIGH); //LED off
    digitalWrite(red,HIGH); //LED off
    digitalWrite(blue,HIGH); //LED off
    
      //}
    }
} //loop


boolean readAnalogSensor() { /* function  */
  static bool PulseDetected;// = 0;
  static bool flag;// = 0;
  //measured_value = analogRead(aisPin);
  //if (measured_value<50) {
  if (analogRead(aisPin)<50 && !flag) {//flag==0) {  
    PulseDetected=1;
    flag=1;
    }
  else {
    PulseDetected=0;
  }

  if (analogRead(aisPin)>1012 && PulseDetected==0) {
    flag=0;
  }  

  return PulseDetected;
}




