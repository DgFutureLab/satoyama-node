#include "TimerOne.h"
boolean high = false;

const float FREQ = 10;
const float period = 1.0/FREQ*1000000.0;
const int n_periods = 10;

void chirp(){
  Timer1.pwm(10, 512);
  float delay_microseconds = period * n_periods;
  if(delay_microseconds <= 16383){
    delayMicroseconds(delay_microseconds);
  } else{
    delay(delay_microseconds/1000);
  }
  
  Timer1.disablePwm(10);
}



//void interrupt(){
//  if (high){
//    digitalWrite(10, LOW);
//    high = false;
//  } else{
//    digitalWrite(10, HIGH);
//    high = true;
//  }
//  Serial.println("Interrupt!!");
//}

void setup() {
  // put your setup code here, to run once:
//  pinMode(10, OUTPUT);
  Serial.begin(112500);
  Serial.print("Setting timer with a period of ");
  Serial.print(period);
  Serial.println(" microseconds");
  Timer1.initialize(period);
//  Timer1.pwm(10, 512);
//  Timer1.attachInterrupt(interrupt);
  Serial.println("Setup done");
}

void loop() {
  Serial.println("period");
  Serial.println(period);
  chirp();
  delay(2000);
  // put your main code here, to run repeatedly: 
//  Serial.println("asdasd");
}
