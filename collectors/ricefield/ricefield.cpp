/*
Collects data from different sensors and sends it to 
 the aggregator as defined in "config.h"
 */

#include "ricefield.h"
#include <chibi.h>
#include <saboten.h>
#include <sensors.h>

// satoyama-chibi-lib includes
// Defines pin numbers for sensors and also a simple format to send
// information to the satoyama edge router
#include <satoyama_config.h>
#include <utilsawesome.h>

// Saboten includes
#include <avr/sleep.h>
#include <avr/power.h>
#include <avr/wdt.h>
#include <Wire.h>
#include <SPI.h>
#include <SdFat.h>
#include <pcf2127.h>

 //Temperature and humidity library
#include "DHT.h"
#define DHTTYPE DHT11   // Type of DHT sensor, in our case we are using DHT11
#define DHT11_PIN A0    // Pin where the DHT11 is connected
dht DHT;

//Sonnar library
#include <NewPing.h>
NewPing sonar(SONAR_PIN,SONAR_PIN,200);


int HIGH_GAIN_MODE_PIN = 14;
int SD_CHIPSELECT_PIN = 15;
int RTC_CHIPSELECT_PIN = 28; 
int LED_PIN = 18;
int SD_DETECT_PIN = 19; 
int vbatPin = 31;
int vsolPin = 29;

uint8_t temp;
PCF2127 pcf(0, 0, 0, RTC_CHIPSELECT_PIN, &temp);
Saboten board;

/**************************************************************************/
// Initialize
/**************************************************************************/
//int interruptPin = 6;

void setup()
{
  // Initialize the chibi command line and set the speed to 57600 bps
  
  chibiCmdInit(57600);
  chibiInit();

  // set up rtc chip select
  pinMode(RTC_CHIPSELECT_PIN, OUTPUT);
  digitalWrite(RTC_CHIPSELECT_PIN, HIGH);

  pinMode(SD_CHIPSELECT_PIN, INPUT);
  digitalWrite(SD_CHIPSELECT_PIN, HIGH);

  pinMode(SD_DETECT_PIN, INPUT);
  digitalWrite(SD_DETECT_PIN, LOW);

  pinMode(HIGH_GAIN_MODE_PIN, OUTPUT);
  digitalWrite(HIGH_GAIN_MODE_PIN, LOW);

  pcf.enableMinuteInterrupt();
  // pcf.enableSecondInterrupt();
  pcf.setInterruptToPulse();

  attachInterrupt(2, board.rtcInterrupt, FALLING);
}

/**************************************************************************/
// Loop
/**************************************************************************/

void loop()
{ 

  // board.sleep_mcu();
  // board.wakeup_radio();
  read_sensors();
  delay(1000);
  // board.sleep_radio();
}


void read_sensors(){
  Serial.println("Reading sensors...");

  uint8_t tx_buf[TX_LENGTH];
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


  Sensors::read_battery_voltage(tx_buf, board.BATTERY_VOLTAGE_PIN, board.ADC_REFERENCE_VOLTAGE);

  
  // Debug print
  Serial.println((char*) tx_buf);

  Serial.println("Transmitting data...");
  

  //Send data stored on "tx_buf" to aggregator (Satoyama edge router)
  chibiTx(AGGREGATOR_SHORT_ADDRESS, tx_buf, TX_LENGTH);
  // chibiTx(20, tx_buf, TX_LENGTH);

  //Wait
  // delay(TX_INTERVAL);
  free(tx_buf);
  Serial.println("Done transmitting...");
}


