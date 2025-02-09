//Parameters
//Working version TEST 
const int aisPin  = A4; //a0-mini.... a4-nano
const int numReadings  = 200;
int readings [numReadings];
int readIndex  = 0;
long total  = 0;
int limitWish = 0; 

long blinkcounter_hour = 0;
int pluss = 0;
int watt = 0;
int over5kw_accu = 0; //when consumtion is over 5kW (KWh_warning) NB! int gives max 32767 i.e. 32kwh in one hour maximum
int pause_peak_accu = 0;

int accu_limit = 1000; //if over5kw_accu > accu_limit.  
int KWh_limit = 4000;
int KWh_warning = 5000;
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
long tid_watt;
 
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

  digitalWrite(blue,HIGH); //HIGH = LED avslått
  digitalWrite(red,HIGH);
  digitalWrite(green,HIGH);

}

void loop() {

if (millis() > refresh_time_hour) {  //om det gått en timme
refresh_time_hour = millis() + Count_result_after_one_hour_passed;
  Serial.print(F("kwh brukt denne timen: "));
  Serial.println(kw_per_pulse*blinkcounter_hour);
pluss=0;  //teller opp minutter. Reset every whole hour
blinkcounter_hour = 0; //number of "0,001Wh" reset every hour
over5kw_accu = 0; //reset every hour
pause_peak_accu = 0;

}   //refresh_time_hour  


if (millis() > refresh_time) {  //om det gått ett minutt legg til en minutt(pluss) få hur langt in i timen som har passerat
refresh_time = millis() + Count_result_after_this_time_passed;

 Serial.println (" ");
 Serial.print(F("kwh brukt siste "));
 Serial.print(pluss);//pluss=((hour/1000/60/60)+pluss-1);
  //pluss++; //pluss er antal minutt etter times-reset
 Serial.print(F(" minutt: "));
 Serial.print(kw_per_pulse*blinkcounter_hour);
 Serial.println(F(" kWh"));

  ////Running average ?
  Serial.println (" ");  
  Serial.print(F("predict_hour_by_average: "));
predict_hour_by_average  = kw_per_pulse*blinkcounter_hour/pluss*60;
pluss++;
  Serial.print (predict_hour_by_average);
  Serial.println (F(" kWh")); 
 // if (predict_hour_by_average<2000 && pluss>30)
  //Serial.println("Watt now: ");
  Serial.println("running average: ");
}   //refresh_time
 

 // readAnalogSensor();

if ((readAnalogSensor()==1) && (flag_count == 0)) {
    //if ((measured_value<50) && (flag_count == 0)) {//measured_value<80 (org) (31 er grensen 8feb2025) //om det kommer en blinkpuls fra e-meter så teller 
                                                    //blinkcounter_hour opp en Watt-timme. dvs 0.001 kwh
                                                    //LED på e-meter lyser, en pulse är detekterad
    
      watt = 3600000/(millis()-tid_watt); //effekt på denna ena pulsen
      blinkcounter_hour++; // antall blink på 0,001 kwh dvs 1 Wh per blink-puls
      
      //#1 Green_state

      if (watt<KWh_warning && blinkcounter_hour<KWh_limit) {
      digitalWrite(green,LOW); //LOW = lyser
      //pause_peak_accu++;
      }

      //#2 Red_state
      if (watt>=KWh_warning) 
      digitalWrite(red,LOW); //LOW = lyser

//TEST
      limitWish = runningAverage(); //runningAverage returnerer flytand gjennomsnitt
      //Serial.println(runningAverage());       Serial.print(" ");
      if ((limitWish>=5500) && (blinkcounter_hour<KWh_limit)) {
      //if ((runningAverage()>=KWh_warning) && blinkcounter_hour<KWh_limit) {
      digitalWrite(blue,LOW); //LOW = lyser
      digitalWrite(cutoff_relay,HIGH);  //energizing coil on NC relay -> cut power to contactors on boiler and waterheater
      }
      if (limitWish<=4500 && blinkcounter_hour<KWh_limit) {
      digitalWrite(cutoff_relay,LOW);
      }
//TEST

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
      if (blinkcounter_hour>=KWh_limit) {
        digitalWrite(blue,LOW); //LOW = lyser
        digitalWrite(cutoff_relay,HIGH); //energizing coil on NC relay -> cut power to contactors on boiler and waterheater
      } 
      //else digitalWrite(cutoff_relay,LOW); 
      if (blinkcounter_hour==1) digitalWrite(cutoff_relay,LOW); 
      //Serial.print(F("Watt / Pulse siste time(Watt Timer): ")); Serial.print(watt); Serial.print(F(" / ")); Serial.println(blinkcounter_hour);
      //Serial.print (".");
      //Serial.print(F("Watt: ")); 
      //Serial.print(watt);       Serial.print(" ");
     //////////// Serial.println(runningAverage());       Serial.print(" ");
      flag_count = 1; 
      tid_watt = millis();
      watt= 0;
      } // (readAnalogSensor()==1)

      if (measured_value > 1012) {
      digitalWrite(green,HIGH); //LED off
      digitalWrite(red,HIGH); //LED off
      digitalWrite(blue,HIGH); //LED off
      flag_count = 0; //ready for new pulse from e-meter.
      }
} //loop


boolean readAnalogSensor() { /* function  */
  boolean PulseDetected;
  measured_value = analogRead(aisPin);
  if (measured_value<50) {
    PulseDetected=1;
  }
  else
  PulseDetected=0;

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
