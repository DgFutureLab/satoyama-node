#define trigPin 13
#define echoPin 12

int old_distance;
int new_distance;
int change;
const int tolerance = 30;
const int ping_delay_msec = 2000;
const int dist_n_avr = 10;
boolean occupied;

void setup() {
  Serial.begin (9600);
  Serial.println("BEGIN");
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}


void loop() {
  new_distance = measure_distance();
  delay(ping_delay_msec);
  old_distance = new_distance;
  new_distance = measure_distance();
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
