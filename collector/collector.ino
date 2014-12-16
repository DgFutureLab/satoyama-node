
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

  chibiCmdAdd("getsaddr", cmdGetShortAddr);  // set the short address of the node
  chibiCmdAdd("setsaddr", cmdSetShortAddr);  // get the short address of the node
  
  Serial.println("Type,\tstatus,\tHumidity (%),\tTemperature (C)");
}

/**************************************************************************/
// Loop
/**************************************************************************/
void loop()
{
  chibiCmdPoll();
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

  //Send data stored on "tx_buf" to aggregator (Satoyama edge router)
  chibiTx(AGGREGATOR_SHORT_ADDRESS, tx_buf, TX_LENGTH);

  // Debug print
  Serial.println((char*) tx_buf);

  //Wait
  delay(30*1000);
  free(tx_buf);
}




/**************************************************************************/
// USER FUNCTIONS
/**************************************************************************/

/**************************************************************************/
/*!
    Get short address of device from EEPROM
    Usage: getsaddr
*/
/**************************************************************************/
void cmdGetShortAddr(int arg_cnt, char **args)
{
  int val;
  
  val = chibiGetShortAddr();
  Serial.print("Short Address: "); Serial.println(val, HEX);
}

/**************************************************************************/
/*!
    Write short address of device to EEPROM
    Usage: setsaddr <addr>
*/
/**************************************************************************/
void cmdSetShortAddr(int arg_cnt, char **args)
{
  int val;
  
  val = chibiCmdStr2Num(args[1], 16);
  chibiSetShortAddr(val);
}


/**************************************************************************/
/*!
    Concatenate multiple strings from the command line starting from the
    given index into one long string separated by spaces.
*/
/**************************************************************************/
int strCat(char *buf, unsigned char index, char arg_cnt, char **args)
{
    uint8_t i, len;
    char *data_ptr;

    data_ptr = buf;
    for (i=0; i<arg_cnt - index; i++)
    {
        len = strlen(args[i+index]);
        strcpy((char *)data_ptr, (char *)args[i+index]);
        data_ptr += len;
        *data_ptr++ = ' ';
    }
    *data_ptr++ = '\0';

    return data_ptr - buf;
}

