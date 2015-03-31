#include "ricefield.h"
#include <saboten.h>
#include <chibi.h>

#include <satoyama_config.h>
// #include <utilsawesome.h>

// Saboten includes
// #include <avr/sleep.h>
// #include <avr/power.h>
// #include <avr/wdt.h>
// #include <Wire.h>
// #include <SPI.h>
// #include <SdFat.h>
// #include <pcf2127.h>

 //Temperature and humidity library



Saboten *board = new Saboten(57600);


void setup()
{
  board->register_sensor(new Paralax28015REVC_Sensor(6));  
  chibiCmdInit(57600);
}

/**************************************************************************/
// Loop
/**************************************************************************/

void loop()
{ 
  uint8_t tx_buf[TX_LENGTH];
  memset(tx_buf, 0, TX_LENGTH);
  board->read_sensors(tx_buf);
  Serial.println((char*) tx_buf);
  chibiTx(AGGREGATOR_SHORT_ADDRESS, tx_buf, TX_LENGTH);
  free(tx_buf);
  Serial.println("Done transmitting...");
  delay(1000);
}


