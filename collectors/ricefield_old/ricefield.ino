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

//Temperature and humidity library
#include "DHT.h"
#define DHTTYPE DHT11   // Type of DHT sensor, in our case we are using DHT11
#define DHT11_PIN A0    // Pin where the DHT11 is connected
dht DHT;

//Sonnar library
#include <NewPing.h>
NewPing sonar(SONAR_PIN,SONAR_PIN,200);

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
  Serial.println("Type,\tstatus,\tHumidity (%),\tTemperature (C)");
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
  free(tx_buf);
}

float VbatRead(){
  float batt;
  unsigned int vbat = analogRead(VBAT_PIN);
  
  batt = ((vbat/1023.0) * ADCREFVOLTAGE) * 2;
  return batt;
}
