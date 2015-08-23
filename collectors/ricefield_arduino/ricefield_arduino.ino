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
#include "DHT.h"
#include <string.h>
// Saboten includes
#include <avr/sleep.h>
#include <avr/power.h>
#include <avr/wdt.h>
#include <Wire.h>
#include <SPI.h>
#include <SdFat.h>
#include <pcf2127.h>

//Temperature and humidity library

#include "dht11.h"
dht11 DHT;
#define DHT11_PIN 4

#define TEMPERATURE_SENSOR_ID "4"
#define HUMIDITY_SENSOR_ID "5"
#define SONAR_SENSOR_ID "3"
#define VBAT_SENSOR_ID "2"

#define NODE_ID 2
#define EDGE_ID 1

//Sonnar library
#include <NewPing.h>
NewPing sonar(6,6,200);

/**************************************************************************/
// Initialize
/**************************************************************************/
char node_key[10];
void setup()
{
  // Initialize the chibi command line and set the speed to 57600 bps
//  chibiCmdInit(115200);
//  chibiSetShortAddr(2);
  // Initialize the chibi wireless stack
  Serial.begin(115200);

  sprintf(node_key, "%d", NODE_ID);
  strcat(node_key, "@");
  Serial.println(node_key);
  chibiInit();
  pinMode(VBAT_PIN, INPUT);
  Serial.println("Type,\tstatus,\tHumidity (%),\tTemperature (C)");
}

/**************************************************************************/
// Loop
/**************************************************************************/


//char str[80];
//printf(str, "Value of Pi = %f", 2);

//char data_len_str[15];
//sprintf(data_len_str, "%d", 10);

void loop()
{
  byte tx_buf[TX_LENGTH];
  memset(tx_buf, 0, TX_LENGTH);
  long duration, inches, cm;
  
  printf((char*)tx_buf, node_key);

  // Read temperature
//  float temperature = DHT.temperature;
//  Serial.println(temperature);  

  read_dht((char*)tx_buf);
  read_vbat((char*) tx_buf);
  read_sonar((char*) tx_buf);
  // Read sonar distance

  Serial.println((char*) tx_buf);
  chibiTx(EDGE_ID, tx_buf, TX_LENGTH);

  //Wait
  delay(1000);
  free(tx_buf);
}

//void read_sonar(cha)

void read_sonar(char *tx_buf){
  float distance = sonar.ping() / US_ROUNDTRIP_CM; 
  if (distance > 0) {
    Reading dist = {SONAR_SENSOR_ID, distance, millis()};
    add_to_tx_buf((char*)tx_buf, &dist);
  }
}

void read_vbat(char *tx_buf){
  unsigned int vbat_int = analogRead(VBAT_PIN);
  float vbat = ((vbat_int/1023.0) * ADCREFVOLTAGE) * 2;
  Reading battery_voltage = {VBAT_SENSOR_ID, vbat, millis()};
  add_to_tx_buf((char*) tx_buf, &battery_voltage);
}


void read_dht(char* buf){
  int chk;
//  Serial.print("DHT11, \t");
  chk = DHT.read(DHT11_PIN);    // READ DATA
  switch (chk){
    case DHTLIB_OK:  
//                Serial.print("OK,\t"); 
                break;
    case DHTLIB_ERROR_CHECKSUM: 
//                Serial.print("Checksum error,\t"); 
                break;
    case DHTLIB_ERROR_TIMEOUT: 
//                Serial.print("Time out error,\t"); 
                break;
    default: 
//                Serial.print("Unknown error,\t"); 
                break;
  }
 // DISPLAT DATA
  Reading temp = {TEMPERATURE_SENSOR_ID, DHT.temperature, millis()};
  add_to_tx_buf((char*) buf, &temp);
  Reading humidity = {HUMIDITY_SENSOR_ID, DHT.humidity, millis()};
  add_to_tx_buf((char*) buf, &humidity);

}

