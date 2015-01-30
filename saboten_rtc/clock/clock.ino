#include <chb.h>
#include <chb_aes.h>
#include <chb_buf.h>
#include <chb_cmd.h>
#include <chb_drvr.h>
#include <chb_eeprom.h>
#include <chb_rx_int.h>
#include <chb_spi.h>
#include <types.h>
#include <SPI.h>
/*
Collects data from different sensors and sends it to 
the aggregator as defined in "config.h"
*/

#include <chibi.h>

// satoyama-chibi-lib includes
// Defines pin numbers for sensors and also a simple format to send
// information to the satoyama edge router
// #include <config.h>
// #include <utilsawesome.h>

// Saboten includes
// #include <avr/sleep.h>
// #include <avr/power.h>
// #include <avr/wdt.h>
// #include <Wire.h>
// #include <SPI.h>
// #include <SdFat.h>
// #include <pcf2127.h>

// PCF2127 pcf(0, 0, 0, rtcCsPin, &temp);

/**************************************************************************/
// Initialize
/**************************************************************************/


void setup()
{
  // Initialize the chibi command line and set the speed to 57600 bps
   chibiCmdInit(57600);
  
  // set up rtc chip select
//  pinMode(rtcCsPin, OUTPUT);
//  digitalWrite(rtcCsPin, HIGH);

  
  // pinMode(interruptPin, INPUT);
//  chibiInit();


}

/**************************************************************************/
// Loop
/**************************************************************************/
int counter = 0;
boolean setTimer = true;
void loop()
{
  Serial.println("Hello"); 
}


