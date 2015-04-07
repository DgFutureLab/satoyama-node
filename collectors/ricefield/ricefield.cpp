// rm -r * && cmake .. && make && make upload && screen -S saboten /dev/tty.usbserial-A501K9HW 57600,sc8

#include "ricefield.h"
#include <saboten.h>
#include <chibi.h>
#include <satoyama_config.h>

Saboten *board = new Saboten(57600);

void setup()
{
  board->register_sensor(new Paralax28015REVC_Sensor(6));  
  chibiCmdInit(57600);
}

void loop()
{ 
  uint8_t tx_buf[TX_LENGTH];
  memset(tx_buf, 0, TX_LENGTH);
  board->read_sensors(tx_buf);
  Serial.println((char*) tx_buf);
  chibiTx(AGGREGATOR_SHORT_ADDRESS, tx_buf, TX_LENGTH);
  free(tx_buf);
  Serial.println("Done transmitting...");
  board->sleep_mcu();
}


