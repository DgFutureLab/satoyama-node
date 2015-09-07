//Arduino UNO code for ultrasonic sensor testing electronic brain
#define trig 7
#define echo 6


void setup() {
  // put your setup code here, to run once:
  Serial.begin (9600);
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);

 

}

void loop() {
  // put your main code here, to run repeatedly:
    long duration, distance;
  digitalWrite(trig, LOW); 
  delayMicroseconds(10);
  digitalWrite(trig, HIGH);
  delayMicroseconds(15);
  digitalWrite(trig, LOW);
  delayMicroseconds(20);
  
  duration = pulseIn(echo, HIGH);
  distance = (duration/2) / 29.1;
  
  Serial.println(distance);
 

  

}
