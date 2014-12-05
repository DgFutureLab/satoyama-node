#include "utilsawesome.h"
/*
 *    Concatenates the information from one Reading to the tx_buf
 *
 *    The format of one reading string is:
 *        reading->name:reading->value:reading->timestamp;
 *
 *    With real information coming from sensors should look like:
 *        "distance:205.00:5623;"
 *
 *    When many Readings have been attached having called this function several times
 *      this is how the tx_buf looks like:
 *        "distance:205.00:18323;temperature:24.00:18323;humidity:45.10:1832"
 *
 */

void add_to_tx_buf(char *tx_buf, struct Reading *reading)
{  
  char buf[10];
  char timestamp_buf[20];
  char str[80];
  char *current_value = dtostrf(reading->value, 2, 2, (char *)buf);

  strcpy(str, "");
  strcat(str, (char *) reading->name);
  strcat(str, ":");
  strcat(str, current_value);
  strcat(str, ":");
  sprintf(timestamp_buf, "%d", reading->timestamp);
  strcat(str, timestamp_buf);
  strcat(str, ";");
  strcat(str, "\0");
  strcat((char *)tx_buf,(char *) str);
}