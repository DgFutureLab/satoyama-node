#define trigPin 13
#define echoPin 12

int old_distance;
int new_distance;
int change;
const int tolerance = 30;
boolean occupied;

void setup() {
  Serial.begin (9600);
  Serial.println("BEGIN");
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}


void loop() {
  new_distance = measure_distance();
  Serial.print("Before: ");
  Serial.println(new_distance);
  delay(2000);
  old_distance = new_distance;
  new_distance = measure_distance();
  Serial.print("After: ");
  Serial.println(new_distance);
  change = new_distance - old_distance;
  Serial.print("Change: ");
  Serial.println(change);    
  if (abs(change) > tolerance){
    if(change > 0){
      occupied = 0;
      Serial.print("Unoccupied");
    } else{
      occupied = 1;
      Serial.print("Occupied");
    }
  }
  Serial.println("");
  Serial.println("");
  
}



int measure_distance(){
  long duration, distance;
  distance = 0;
  for(int i = 0; i<10; i++){
    digitalWrite(trigPin, LOW);  // Added this line
    delayMicroseconds(2); // Added this line
    digitalWrite(trigPin, HIGH);
  //  delayMicroseconds(1000); - Removed this line
    delayMicroseconds(10); // Added this line
    digitalWrite(trigPin, LOW);
    duration = pulseIn(echoPin, HIGH);
    distance += (duration/2) / 29.1;
  }
  distance /= 10;
  Serial.println(distance);
  return distance;
}
