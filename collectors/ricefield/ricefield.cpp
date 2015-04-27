// rm -r * && cmake .. && make && make upload && screen -S saboten /dev/tty.usbserial-A501K9HW 57600,sc8
#include <string>
#include "ricefield.h"
#include <saboten.h>
#include <chibi.h>
#include <satoyama_config.h>
// #include <SPI.h>
// #include <SD.h>

#include <SPI.h>
#include <SdFat.h>

#define FILENAME "data.txt"

SdFat sd;
SdFile myFile;

Saboten *board = new Saboten(57600);

int sdDetectPin = 19;
int sdCsPin = 15;

void setup()
{
  board->register_sensor(new Paralax28015REVC_Sensor(5));  
  // board->register_sensor(new DHT_V12_Sensor(6));  
  chibiCmdInit(57600);
  init_sdcard();
  

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
  writeData(tx_buf);
  
  free(tx_buf);
  delay(10000);
  
  
  for(int i = 0; i < 1; i++){
    Serial.println("Sleeping");
    board->sleep_mcu();
    Serial.println("Awake");
  }
  // board->sleep_mcu();  
  

}

void init_sdcard(){
// set up sd card detect
  pinMode(sdDetectPin, INPUT);
  digitalWrite(sdDetectPin, HIGH);
  // delay(1000);
  // check for SD and init
  int sdDetect = digitalRead(sdDetectPin);
  // Serial.println(sdDetect);
  if (sdDetect == 0){
    // init the SD card
    if (!sd.begin(sdCsPin)){
      
      Serial.println("Card failed, or not present");
      sd.initErrorHalt();
      return;
    }
    Serial.println("SD Card is initialized.\n");
  }
  else{
    Serial.println("No SD card detected.\n");
  }

}


void writeData(unsigned char *buffer){    
    int open_success = myFile.open(FILENAME, O_RDWR | O_CREAT | O_AT_END);
    // Serial.print("Open file: ");
    // Serial.println(open_success);
    if (open_success){
      // std::string str(buffer);
      
      char data[100];
      strcpy(data, (char*) buffer);
      Serial.println("Writing data to SD card ");
      // Serial.println(data); 
      // int n = sizeof(buffer);
      // Serial.println(n);

      
      myFile.println(data);

      // char * ptr;
      // char array[10];
      // // Populate ptr
      // ptr = "10234";
      // // Copy from ptr to array.
      // strcpy(array, ptr);      
      // Serial.println("Wrote data");

      myFile.close();
      Serial.println("Closed file");
     
    }
    else{
      Serial.println("Error opening dataFile");
    }
}


