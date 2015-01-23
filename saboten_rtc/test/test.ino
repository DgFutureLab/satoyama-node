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
int interruptPin = 6;

void setup()
{
  // Initialize the chibi command line and set the speed to 57600 bps
  chibiCmdInit(57600);
  
  // set up rtc chip select
//  pinMode(rtcCsPin, OUTPUT);
//  digitalWrite(rtcCsPin, HIGH);

  
  pinMode(interruptPin, INPUT);
//  chibiInit();

  
  attachInterrupt(2, testInterrupt, FALLING);
}

/**************************************************************************/
// Loop
/**************************************************************************/
int counter = 0;
boolean setTimer = true;
void loop()
{

  int reading = digitalRead(interruptPin);
//  Serial.println(reading);
  
  uint8_t hour, minute, second;
  pcf.readTime(&hour, &minute, &second);
  char time_buffer [50];
  sprintf (time_buffer, "Now is: %u-%u-%u. Pin 6: %d", hour, minute, second, reading);
  
//  Serial.println((char*) time_buffer);
  delay(1000);
//  if (setTimer == true){
//    setTimer = false;
//    setWakeupTime();
//    readTimerRegisters();
//  }
// bcdTest();
 setWakeupTime();
  
}

void readTimerRegisters(){
  char buffer [100];
  uint8_t hour_set, minute_set, second_set;
  pcf.alarmReadTime(&hour_set, &minute_set, &second_set);
  sprintf(buffer, "ALARM TIME IN REGISTERS: hour: %u, minute: %u, second: %u", hour_set, minute_set, second_set);
  Serial.println((char*) buffer);
  
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

void setWakeupTime(){
  /*
    alarmEnb(1,0)
    void alarmWriteDate(uint8_t day, uint8_t weekday = 0);    
    void alarmEnb(uint8_t almTimeEnb, uint8_t almDayEnb, uint8_t almWeekdayEnb = 0);
  */
  
  
  /*
    THIS FUNCTION DISABLES THE ALARM
  */
//  pcf.alarmEnb(1, 0, 0); //use time alarm, but not day or weekday alarms
  
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
  
  char read_buffer [100];
  uint8_t hour_set, minute_set, second_set;
  pcf.alarmReadTime(&hour_set, &minute_set, &second_set);
  sprintf(read_buffer, "ALARM TIME IN REGISTERS: hour: %u, minute: %u, second: %u", hour_set, minute_set, second_set);
  Serial.println((char*) read_buffer);

  
  
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
