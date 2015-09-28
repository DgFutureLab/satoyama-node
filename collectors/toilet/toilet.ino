#define NODE_ID 4
#define SONAR_SENSOR_ID 10

/*
 Constants and variables for sonar stuff
*/

#define trigPin 13
#define echoPin 12
int old_distance = 0;
int new_distance = 0;
int change;
int toilet_occupied;
const int tolerance = 30;
const int ping_delay_msec = 2000;
const int dist_n_avr = 10;
boolean occupied;



/*
 Constants and variables for 3G stuff
*/

#include <SoftwareSerial.h>
#include "a3gim.h"

const char *server = "satoyamacloud.com";
int port = 80;
char *path;
char res[a3gsMAX_RESULT_LENGTH+1];
int len;



#include <TimerOne.h>



//void get_toilet_status(){
////  Serial.println("A");
//    new_distance = measure_distance();
//  delay(ping_delay_msec);
//  old_distance = new_distance;
//  new_distance = measure_distance();
//  change = new_distance - old_distance;
//
//  if (abs(change) > tolerance){
//    if(change > 0){
//      occupied = 0;
//      Serial.println("Unoccupied");
//    } else{
//      occupied = 1;
//      Serial.println("Occupied");
//    }
//    send_toilet_status(occupied);
//  }
//  
//}

void interrupt(){
  old_distance = new_distance;
  new_distance = measure_distance();
}


void setup() {
  Serial.begin(115200);
  Serial.println("BEGIN");
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  init_3G_connection();
  Timer1.initialize(5000000);
  Timer1.attachInterrupt(interrupt);
}


void loop() {
  change = new_distance - old_distance;
  if (abs(change) > tolerance){
    if(change > 0){
      occupied = 0;
      Serial.println("Unoccupied");
    } else{
      occupied = 1;
      Serial.println("Occupied");
    }
    send_toilet_status(occupied);
  }
}



void send_toilet_status(boolean occupied){
//  char path[100];
  if(occupied){
    path = "/reading/1?sensor_id=10&value=1";
//    printf("/reading/%u?sensor_id=4&value=1", NODE_ID);
  } else{
    path = "/reading/1?sensor_id=10&value=0";
  }
 
 if (a3gs.httpGET(server, port, path, res, len) == 0) {
     Serial.println("HTTP OK"); 
     Serial.println(res);
    }
    else {
      Serial.print("Can't get HTTP response from ");
      Serial.println(server);
    } 
  
}

void init_3G_connection(){
  Serial.println("Starting 3G shield...");
  if (a3gs.start() == 0 && a3gs.begin() == 0) {
  Serial.println("Succeeded.");
  } else{
    Serial.println("Failed.");
  }
}



int measure_distance(){
  long duration, distance;
  distance = 0;
  for(int i = 0; i<dist_n_avr; i++){
    digitalWrite(trigPin, LOW);  // Added this line
    delayMicroseconds(2); // Added this line
    digitalWrite(trigPin, HIGH);
  //  delayMicroseconds(1000); - Removed this line
    delayMicroseconds(10); // Added this line
    digitalWrite(trigPin, LOW);
    duration = pulseIn(echoPin, HIGH);
    distance += (duration/2) / 29.1;
  }
  distance /= dist_n_avr;
  return distance;
}
