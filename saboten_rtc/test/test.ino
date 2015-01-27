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

int sdDetectPin = 19;

int rtcCsPin = 28; 
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

  testSecondInterrupt();
//  pinMode(interruptPin, INPUT);
//
//  attachInterrupt(2, testInterrupt, CHANGE);
}

/**************************************************************************/
// Loop
/**************************************************************************/
int counter = 0;
boolean setTimer = true;
void loop()
{

  delay(1000);
  byte msf = pcf.read(PCF_CONTROL_2);
  Serial.print("MSF: ");
  Serial.println(msf, BIN);
//  testSecondInterrupt();
}

void testSecondInterrupt(){
  enableSecondInterrupt();
  setInterruptToPulse();
}


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
//  byte RC2 = pcf.read(PCF_CONTROL_2);
//  setControlBit(&RC1, BIT_MI, 0); // second interrupt
//  setControlBit(&RC1, BIT_SI, 1); // second interrupt
//  Serial.print("Set control register 1 to: ");
//  Serial.println(RC1, BIN);
//  pcf.write(PCF_CONTROL_1, RC1);
//  


void setupControlRegisters(){
//  byte RC1 = 0;
//  setControlBit(&RC1, BIT_EXT_TEST, 0); //Normal mode
//  setControlBit(&RC1, BIT_STOP, 0); // source clock runs
//  setControlBit(&RC1, BIT_TSF_1, 0); // no timestamp interrupts are generated
//  setControlBit(&RC1, BIT_POR_OVRD, 0); // power on reset normal mode
//  setControlBit(&RC1, BIT_12_24, 0); // set to 24h mode 
//  setControlBit(&RC1, BIT_MI, 0); // no minute interrupt
//  setControlBit(&RC1, BIT_SI, 0); // no second interrupt
//  Serial.print("Set control register 1 to: ");
//  Serial.println(RC1, BIN);
//  pcf.write(PCF_CONTROL_1, RC1);
//
//  byte RC2;
//  setControlBit(&RC2, BIT_MSF, 7); //No minute or second interrupt is generated
//  setControlBit(&RC2, BIT_WDTF, 0); //No watchdog interrupts are generated
//  setControlBit(&RC2, BIT_TSF_2, 0); //no timestamp interrupt
//  setControlBit(&RC2, BIT_AF, 0); // flag set when interrupt cleared
//  setControlBit(&RC2, BIT_CDTF, 0); // No countdown timer interrupt will be generated
//  setControlBit(&RC2, BIT_TSIE, 0); // No interrupt generated from timestamp flag
//  setControlBit(&RC2, BIT_AIE, 0); 
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



void testInterrupt(){
  counter++;
  Serial.println("INTERRUPT");    
}


void cmdSleepMcu(int arg_cnt, char **args)
{
  printf("Sleeping MCU\n");
  delay(100);

  // set pullups on inputs
  //  pinMode(sdCsPin, INPUT);
  //nn  digitalWrite(sdCsPin, HIGH);

  //  pinMode(sdDetectPin, INPUT);
  digitalWrite(sdDetectPin, LOW);

  // write sleep mode
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_enable();                       // setting up for sleep ...
  ADCSRA &= ~(1 << ADEN);    // Disable ADC
  sleep_mode();
}


void bcdTest(){
  uint8_t second = B11010001;
  uint8_t mask = B01111111;
  uint8_t masked;
  masked = second & mask;
  Serial.print("Masked (BIN): ");
  Serial.println(masked, BIN); //Expect B01010001
  Serial.print("Masked (DEC): ");
  Serial.println(masked, DEC); //Expect 81
  uint8_t decoded;
  decoded = pcf.bcdDecode(masked);
  Serial.print("Decoded (DEC): ");
  Serial.println(decoded, DEC); //Expect 51
  Serial.print("Decoded (BIN): ");
  Serial.println(decoded, BIN); //Expect B00110011
}




