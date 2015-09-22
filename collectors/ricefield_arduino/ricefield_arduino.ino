/*
Collects data from different sensors and sends it to 
the aggregator as defined in "config.h"
*/

#include <chibi.h>

// satoyama-chibi-lib includes
// Defines pin numbers for sensors and also a simple format to send
// information to the satoyama edge router
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

#define TX_LENGTH 100
#define VBAT_PIN 31
#define ADCREFVOLTAGE 3.3

//Temperature and humidity library

#include "dht11.h"
dht11 DHT;
#define DHT11_PIN 4

#define UPDATE_INTERVAL_MSEC 5000

#define TEMPERATURE_SENSOR_ID 4
#define HUMIDITY_SENSOR_ID 5
#define SONAR_SENSOR_ID 3
#define VBAT_SENSOR_ID 2

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
  Serial.begin(115200);
  chibiInit();
  chibiSetShortAddr(NODE_ID);
  sprintf(node_key, "%d", chibiGetShortAddr());
  strcat(node_key, "@");
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
  strcat((char*) tx_buf, node_key);
//  printf((char*)tx_buf, node_key);

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
  delay(UPDATE_INTERVAL_MSEC);
  free(tx_buf);
}

//void read_sonar(cha)

void read_sonar(char *tx_buf){
  float distance = sonar.ping() / US_ROUNDTRIP_CM; 
  if (distance > 0) {
    Reading dist = {NODE_ID, SONAR_SENSOR_ID, distance, millis()};
    append_reading((char*)tx_buf, &dist);
  }
}

void read_vbat(char *tx_buf){
  unsigned int vbat_int = analogRead(VBAT_PIN);
  float vbat = ((vbat_int/1023.0) * ADCREFVOLTAGE) * 2;
  Reading battery_voltage = {NODE_ID, VBAT_SENSOR_ID, vbat, millis()};
  append_reading((char*) tx_buf, &battery_voltage);
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
  Reading temp = {NODE_ID, TEMPERATURE_SENSOR_ID, DHT.temperature, millis()};
  append_reading((char*) buf, &temp);
  Reading humidity = {NODE_ID, HUMIDITY_SENSOR_ID, DHT.humidity, millis()};
  append_reading((char*) buf, &humidity);

}

