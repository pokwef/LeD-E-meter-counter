//Parameters
//Working version TEST 10 feb 19:54
//
const int aisPin  = A4; //a0-mini.... a4-nano
const int numReadings  = 350;
int readings [numReadings];
int readIndex  = 0;
long total  = 0;
int limitWish = 0; 

long blinkcounter_hour = 0;
int minutes_into = 0;
int watt = 0;
int over5kw_accu = 0; //when consumtion is over 5kW (KWh_warning) NB! int gives max 32767 i.e. 32kwh in one hour maximum
int pause_peak_accu = 0;

int accu_limit = 1000; //if over5kw_accu > accu_limit.  
int KWh_limit = 4900; //Wh
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
long refresh_time_hour;
long tid_watt = 0;
 
const long Count_result_after_this_time_passed = minute; //60000= 1 min 
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

if (millis() > refresh_time_hour) {  //om det gått en timme
refresh_time_hour = millis() + Count_result_after_one_hour_passed;
  Serial.print(F("kwh brukt denne timen: "));
  Serial.println(kw_per_pulse*blinkcounter_hour);
minutes_into=0;  //teller opp minutter. Reset every whole hour
blinkcounter_hour = 0; //number of "0,001Wh" reset every hour
over5kw_accu = 0; //reset every hour
pause_peak_accu = 0;

}   //refresh_time_hour  


if (millis() > refresh_time) {  //om det gått ett minutt legg til en minutt(minutes_into) få hur langt in i timen som har passerat
refresh_time = millis() + Count_result_after_this_time_passed;

 Serial.println (" ");
 Serial.print(F("kwh brukt siste "));
 Serial.print(minutes_into);//minutes_into=((hour/1000/60/60)+minutes_into-1);
  //minutes_into++; //minutes_into er antal minutt etter times-reset
 Serial.print(F(" minutt: "));
 Serial.print(kw_per_pulse*blinkcounter_hour);
 Serial.println(F(" kWh"));

  ////Running average ?
  Serial.println (" ");  
  Serial.print(F("predict_hour_by_average: "));
predict_hour_by_average = kw_per_pulse*blinkcounter_hour/minutes_into*60;
minutes_into++;
  Serial.print (predict_hour_by_average);
  Serial.println (F(" kWh")); 
 // if (predict_hour_by_average<2000 && minutes_into>30)
  //Serial.println("Watt now: ");
  Serial.println("running average: ");
}   //refresh_time
 

 // readAnalogSensor();

if (readAnalogSensor()==1) { //pulse from e-meter detected.
 
      watt = 3600000/(millis()-tid_watt); //power on this pulse 
      blinkcounter_hour++; // number of pulses so far - in this hour 
      
      //limitWish = runningAverage(); //runningAverage function
      //Serial.println(runningAverage());       Serial.print(" ");


    //#1 Green_state

      if (watt<KWh_warning) {// && blinkcounter_hour<KWh_limit) {
      digitalWrite(green,LOW); //LOW = lyser
      //pause_peak_accu++;
      }

    //#2 Red_state
      if (watt>=KWh_warning) 
      digitalWrite(red,LOW); //LOW = lyser

      /* //TEST
      if ((limitWish>=5500) && (blinkcounter_hour<KWh_limit)) {
      //if ((runningAverage()>=KWh_warning) && blinkcounter_hour<KWh_limit) {
      digitalWrite(blue,LOW); //LOW = lyser
      digitalWrite(cutoff_relay,HIGH);  //energizing coil on NC relay -> cut power to contactors on boiler and waterheater
      }
      if (limitWish<=4500 && blinkcounter_hour<KWh_limit) {
      digitalWrite(cutoff_relay,LOW);
      }
      */ //TEST

/*
      if (predict_hour_by_average>5500){
        over5kw_accu=accu_limit;
      }

      if (watt>=KWh_warning && blinkcounter_hour<KWh_limit) {
        digitalWrite(red,LOW); //LOW = lyser
        over5kw_accu++;
      }
      
      if (over5kw_accu>=accu_limit && over5kw_accu>10) {
            digitalWrite(cutoff_relay,HIGH); //energizing coil on NC relay -> cut power to contactors on boiler and waterheater
            digitalWrite(blue,LOW); //LOW = lyser
            pause_peak_accu++;
          } 
      if (pause_peak_accu>=accu_limit) {
                digitalWrite(cutoff_relay,LOW);
                over5kw_accu=0;
                pause_peak_accu=0;
              }
  */    
//#3 Blue_state
if (blinkcounter_hour>=(KWh_limit-100) || predict_hour_by_average>=KWh_limit) {
  digitalWrite(blue,LOW); //LOW = lyser
  digitalWrite(cutoff_relay,HIGH); //energizing coil on NC relay -> cut power to contactors on boiler and waterheater
}
else digitalWrite(cutoff_relay,LOW);

  //    //#3 Blue_state
  //    if (blinkcounter_hour>=KWh_limit) {
  //      digitalWrite(blue,LOW); //LOW = lyser
  //      digitalWrite(cutoff_relay,HIGH); //energizing coil on NC relay -> cut power to contactors on boiler and waterheater
  //    } 
      //else digitalWrite(cutoff_relay,LOW); 
      ////if (blinkcounter_hour==1) digitalWrite(cutoff_relay,LOW); //if there is a new hour. reset relay.
      Serial.print(F("Watt / Pulse siste time(Watt Timer): ")); Serial.print(watt); Serial.print(F(" / ")); Serial.println(blinkcounter_hour);
      //Serial.print (".");
      //Serial.print(F("Watt: ")); 
      //Serial.print(watt);       Serial.print(" ");
     //////////// Serial.println(runningAverage());       Serial.print(" ");
      flag_count = 1; 
      tid_watt = millis();
      watt= 0;
      } // (readAnalogSensor()==1)

else {
    // if (measured_value > 1012) {
      delay(5); //led a bit brighter
    digitalWrite(green,HIGH); //LED off
    digitalWrite(red,HIGH); //LED off
    digitalWrite(blue,HIGH); //LED off
    //flag_count = 0; //ready for new pulse from e-meter.
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



long runningAverage() { // function flytande gjennomsnitt på konsumsjon. Hver pulse. numReadings antal verdier: 200

  ////Perform average on consumtion
  long average;
 
  // subtract the last reading:
  total = total - readings[readIndex];
  // read the consumtion:
  readings[readIndex] = watt;
  // add value to total:
  total = total + readings[readIndex];
  // handle index
  readIndex = readIndex + 1;
  if (readIndex >= numReadings) {
    readIndex = 0;
  }
  // calculate the average:
  average = total / numReadings;
Serial.print("AVG: ");
Serial.println(average); 
  return average;
}


/*
void Green_state() {
//return (relay_state);
}

void Red_state() {

}

void Blue_state() {
  
}
*/
