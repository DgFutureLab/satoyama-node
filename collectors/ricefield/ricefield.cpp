// rm -r * && cmake .. && make && make upload && screen -S saboten /dev/tty.usbserial-A501K9HW 57600,sc8

#include "ricefield.h"
#include <saboten.h>
#include <chibi.h>
#include <satoyama_config.h>
// #include <SPI.h>
// #include <SD.h>

#include <SPI.h>
#include <SdFat.h>

SdFat sd;
SdFile myFile;

Saboten *board = new Saboten(57600);

int sdDetectPin = 19;
int sdCsPin = 15;

void setup()
{
  board->register_sensor(new Paralax28015REVC_Sensor(6));  
  chibiCmdInit(57600);
  init_sdcard();
  

}

void loop()
{ 
  // uint8_t tx_buf[TX_LENGTH];
  // memset(tx_buf, 0, TX_LENGTH);
  // board->read_sensors(tx_buf);
  // Serial.println((char*) tx_buf);
  // chibiTx(AGGREGATOR_SHORT_ADDRESS, tx_buf, TX_LENGTH);
  // free(tx_buf);
  // Serial.println("Done transmitting...");
  // board->sleep_mcu();


}

void init_sdcard(){
// set up sd card detect
  pinMode(sdDetectPin, INPUT);
  digitalWrite(sdDetectPin, HIGH);
  delay(1000);
  // check for SD and init
  int sdDetect = digitalRead(sdDetectPin);
  Serial.println(sdDetect);
  if (sdDetect == 0)
  {
    // init the SD card
    if (!sd.begin(sdCsPin)) 
    {
      
      Serial.println("Card failed, or not present");
      sd.initErrorHalt();
      return;
    }
    Serial.println("SD Card is initialized.\n");
  }
  else
  {
    Serial.println("No SD card detected.\n");
  }

}


