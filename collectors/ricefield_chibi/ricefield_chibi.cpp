// rm -r * && cmake .. && make && make upload && screen -S saboten /dev/tty.usbserial-A501K9HW 57600,sc8
#include <string>
#include "ricefield_chibi.h"
#include <saboten.h>
#include <chibi.h>
#include <satoyama_config.h>
// #include <SPI.h>
// #include <SD.h>

#include <SPI.h>
#include <SdFat.h>
// #include <SPI.h>
// #include <SD.h>

// #include <SPI.h>
// #include <SdFat.h>

// #define FILENAME "data.txt"

// SdFat sd;
// SdFile myFile;

Chibi *board = new Chibi(57600);

// int sdDetectPin = 19;
// int sdCsPin = 15;

void setup()
{
  board->register_sensor(new Paralax28015REVC_Sensor(5));  
  // board->register_sensor(new DHT_V12_Sensor(6));  
  chibiCmdInit(57600);
  // init_sdcard();
  // board->set_datetime(15, 5, 1, 15, 20, 0);
  // char msg[100];
  // memset(msg, 0, 100);
  // sprintf(msg, "Clock set to: %s", (char*)board->timestamp());
  // writeData((unsigned char*)msg);
  

}

void loop()
{ 
  uint8_t tx_buf[TX_LENGTH];
  memset(tx_buf, 0, TX_LENGTH);
  
  board->read_sensors(tx_buf);
  board->read_board_diagnostics(tx_buf);
  
  Serial.print("Data in buffer: ");
  Serial.println((char*) tx_buf);
  
  // chibiTx(AGGREGATOR_SHORT_ADDRESS, tx_buf, TX_LENGTH);
  // writeData(tx_buf);
  
  free(tx_buf);
  // delay(1000);
  
  
  // for(int i = 0; i < 60; i++){
  //   // Serial.println("Sleeping");
  //   board->sleep_mcu();
  //   // Serial.println("Awake");
  // }
  // // board->sleep_mcu();  
  

}