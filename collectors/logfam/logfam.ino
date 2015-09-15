#include <Adafruit_CC3000.h>
#include <ccspi.h>
#include <SPI.h>
#include <string.h>
#include "utility/debug.h"

#define LIGHT0 0
int val0 = 0;

void setup() {
  Serial.begin(9600);
}

void loop() {
  val0 = analogRead(LIGHT0);
  delay(100);
  if (val0 <400 ){
    Serial.println(1);
  }
  else {
    Serial.println(0);
  }
}
