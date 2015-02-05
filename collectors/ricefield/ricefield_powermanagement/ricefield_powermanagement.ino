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

// Saboten Real time clock
#include <pcf2127.h>
int rtcCsPin = 28;
uint8_t temp;
PCF2127 pcf(0, 0, 0, rtcCsPin, &temp);

//Temperature and humidity library
#include "DHT.h"
#define DHTTYPE DHT11   // Type of DHT sensor, in our case we are using DHT11
#define DHT11_PIN A0    // Pin where the DHT11 is connected
dht DHT;

// Sonnar library
#include <NewPing.h>
NewPing sonar(SONAR_PIN,SONAR_PIN,200);

// The Others
int sdCsPin = 15;
int sdDetectPin = 19;

/**************************************************************************/
// Initialize
/**************************************************************************/
void setup()
{
  // Initialize the chibi command line and set the speed to 57600 bps
  chibiCmdInit(57600);
  
  // Initialize the chibi wireless stack
  chibiInit();
  pinMode(VBAT_PIN, INPUT);

  // Setup interrupt for RTC
  enableMinuteInterrupt();
  setInterruptToPulse();
  attachInterrupt(2, rtcInterrupt, FALLING);
}

/**************************************************************************/
// Loop
/**************************************************************************/
void loop()
{
  byte tx_buf[TX_LENGTH];
  memset(tx_buf, 0, TX_LENGTH);
  long duration, inches, cm;

  // Read temperature
  float temperature = DHT.temperature;  
  if (temperature > 0) {
    Reading temp = {"temperature", temperature, millis()};
    add_to_tx_buf((char*)tx_buf, &temp);
  }

  // Read humidity
  float humidity = DHT.humidity;
  if (humidity > 0) {
    Reading hum = {"humidity", humidity , millis()};
    add_to_tx_buf((char*)tx_buf, &hum);
  }

  // Read sonar distance
  float distance = sonar.ping() / US_ROUNDTRIP_CM; 
  
  if (distance > 0) {
    Reading dist = {"distance", distance, millis()};
    add_to_tx_buf((char*)tx_buf, &dist);
  }

  // Read battery voltage
  float vbat = VbatRead();
  Reading battery_voltage = {"vbat", vbat, millis()};
  add_to_tx_buf((char*) tx_buf, &battery_voltage);

  // Debug print
  Serial.println((char*) tx_buf);

  //Send data stored on "tx_buf" to aggregator (Satoyama edge router)
  chibiTx(AGGREGATOR_SHORT_ADDRESS, tx_buf, TX_LENGTH);

  //Wait
  delay(TX_INTERVAL);
  sleepMCU();
  free(tx_buf);
}

float VbatRead(){
  float batt;
  unsigned int vbat = analogRead(VBAT_PIN);
  
  batt = ((vbat/1023.0) * ADCREFVOLTAGE) * 2;
  return batt;
}


/**
 * Interrupt and real time clock
 */
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

