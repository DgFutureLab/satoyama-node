/*
Collects data from different sensors and sends it to 
 the aggregator as defined in "config.h"
 */

#include <chibi.h>

// satoyama-chibi-lib includes
// Defines pin numbers for sensors and also a simple format to send
// information to the satoyama edge router
#include <config.h>
#include <utilsawesome.h>

// Saboten includes
#include <avr/sleep.h>
#include <avr/power.h>
#include <avr/wdt.h>
#include <Wire.h>
#include <SPI.h>
#include <SdFat.h>
#include <pcf2127.h>


int hgmPin = 14;
int sdCsPin = 15;
int rtcCsPin = 28; 
int ledPin = 18;
int sdDetectPin = 19;
int vbatPin = 31;
int vsolPin = 29;


uint8_t temp;
PCF2127 pcf(0, 0, 0, rtcCsPin, &temp);

/**************************************************************************/
// Initialize
/**************************************************************************/
//int interruptPin = 6;

void setup()
{
  // Initialize the chibi command line and set the speed to 57600 bps
  chibiCmdInit(57600);

//  testSecondInterrupt();
//  pinMode(interruptPin, INPUT);
//
  enableMinuteInterrupt();
  setInterruptToPulse();

  attachInterrupt(2, rtcInterrupt, FALLING);
}

/**************************************************************************/
// Loop
/**************************************************************************/
void loop()
{

//  delay(1000);
//  byte msf = pcf.read(PCF_CONTROL_2);
//  Serial.print("MSF: ");
//  Serial.println(msf, BIN);
//  delay(1000);
  sleepMCU();
//  testSecondInterrupt();
}

void testSecondInterrupt(){
  enableSecondInterrupt();
  setInterruptToPulse();
}

void rtcInterrupt(){
  detachInterrupt(2);
}

void sleepMCU()
{
  attachInterrupt(2, rtcInterrupt, FALLING);
  Serial.println("Sleeping MCU");
  delay(100);

  // set pullups on inputs
  pinMode(sdCsPin, INPUT);
  digitalWrite(sdCsPin, HIGH);

  pinMode(sdDetectPin, INPUT);
  digitalWrite(sdDetectPin, LOW);

  // write sleep mode
//  sei() //   sleep enable interrupt
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_enable();                       // setting up for sleep ...
  
  ADCSRA &= ~(1 << ADEN);    // Disable ADC
  sleep_mode();

  sleep_disable();
  doStuff();
  

}

void doStuff(){
  
  Serial.println("I'm awake!");
  for(int i = 5; i > 0; i--){
    Serial.print("Going to sleep in ");
    Serial.println(i);
    delay(1000);
  }
  
  
}

//void sleepRadio(int arg_cnt, char **args)
//{
//  int val = strtol(args[1], NULL, 10);
//  
//  if (val)
//  {
//    digitalWrite(hgmPin, LOW);
//  
//    // set up chibi regs to turn off external P/A
//    chibiRegWrite(0x4, 0x20);
//  }
//  else
//  {
//    digitalWrite(hgmPin, HIGH);
//    
//    // set up chibi regs to turn on external P/A
//    chibiRegWrite(0x4, 0xA0);
//  }
//  
//  // turn on/off radio
//  chibiSleepRadio(val);
//}
//
//


void enableSecondInterrupt(){
  byte RC1 = pcf.read(PCF_CONTROL_1);
  setControlBit(&RC1, BIT_MI, 0); // disable minute interrupt
  setControlBit(&RC1, BIT_SI, 1); // enable second interrupt
  pcf.write(PCF_CONTROL_1, RC1);
}  

void enableMinuteInterrupt(){
  byte RC1 = pcf.read(PCF_CONTROL_1);
  setControlBit(&RC1, BIT_MI, 1); // enable minute interrupt
  setControlBit(&RC1, BIT_SI, 0); // disable second interrupt
  pcf.write(PCF_CONTROL_1, RC1);
}  


void setInterruptToPulse(){
  byte reg_10h = pcf.read(PCF_WATCHDOG_TIM_CTL);
  reg_10h |= B00100000;
  pcf.write(PCF_WATCHDOG_TIM_CTL, reg_10h);
}

void setInterruptToPermanent(){
  byte reg_10h = pcf.read(PCF_WATCHDOG_TIM_CTL);
  reg_10h &= B11011111;
  pcf.write(PCF_WATCHDOG_TIM_CTL, reg_10h);
}


byte setControlBit(byte *controlByte, byte registerBit, boolean value){
  /*

  */
  *controlByte |= (value<<registerBit);
}

void readTimerRegisters(){
  char buffer [100];
  uint8_t hour_set, minute_set, second_set;
  pcf.alarmReadTime(&hour_set, &minute_set, &second_set);
  sprintf(buffer, "ALARM TIME IN REGISTERS: hour: %u, minute: %u, second: %u", hour_set, minute_set, second_set);
  Serial.println((char*) buffer);
}

void printTime(){
  char buffer [100];
  uint8_t hour_set, minute_set, second_set;
  pcf.readTime(&hour_set, &minute_set, &second_set);
  sprintf(buffer, "TIME NOW: hour: %u, minute: %u, second: %u", hour_set, minute_set, second_set);
  Serial.println((char*) buffer); 
}


void setWakeupTime(){
  uint8_t hour, minute, second;


  pcf.readTime(&hour, &minute, &second);
  char time_buffer [100];
  sprintf (time_buffer, "CURRENT TIME: %u-%u-%u", hour, minute, second);
  Serial.println((char*) time_buffer);

  second = (second + 5) % 60;
  char wakeuptime_buffer [100];
  sprintf (wakeuptime_buffer, "ALARM TIME TO WRITE: %u-%u-%u", hour, minute, second);
  Serial.println((char*) wakeuptime_buffer);


  pcf.alarmWriteTime(hour, minute, second);
  pcf.readModWriteBit(PCF_CONTROL_2, BIT_AIE, 1);    

}









