#include <Servo.h>

Servo leftServo;
Servo rightServo;

int i = 0;
int value;
int center = 0;
const int dur = 2;
const int pingMid = 7;
const int pingLeft = 8;
const int pingRight = 6;
const int pResistor = A0; // Photoresistor at Arduino analog pin A0
const int ledPin=9;       // Led pin at Arduino pin 9
const int darkResistor = A2;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  pinMode(ledPin, OUTPUT);  // Set lepPin - 9 pin as an output
  pinMode(pResistor, INPUT);// Set pResistor - A0 pin as an input (optional)
  pinMode(darkResistor, INPUT);
  
  leftServo.attach(11);
  rightServo.attach(10);

  leftServo.writeMicroseconds(1500);
  rightServo.writeMicroseconds(1500);

//  for(i = 0; i < dur * 100; i++) {
//    leftServo.writeMicroseconds(1750);
//    rightServo.writeMicroseconds(1750);
//    delay(15);
//  }
//
//  leftServo.writeMicroseconds(1500);
//  rightServo.writeMicroseconds(1500);

}

void loop() {
  // put your main code here, to run repeatedly:
  long durMid, mid;

  pinMode(pingMid, OUTPUT);
  digitalWrite(pingMid, LOW);
  delayMicroseconds(2);
  digitalWrite(pingMid, HIGH);
  delayMicroseconds(5);
  digitalWrite(pingMid, LOW);

  

  // The same pin is used to read the signal from the PING))): a HIGH pulse
  // whose duration is the time (in microseconds) from the sending of the ping
  // to the reception of its echo off of an object.
  pinMode(pingMid, INPUT);
  
  pinMode(pingRight, INPUT);
  durMid = pulseIn(pingMid, HIGH);

  mid = microsecondsToInches(durMid);
  
  Serial.print("Middle: ");
  Serial.print(mid);
  Serial.print("in\n");
  
  bool light = checkLight();

  if(light) {
    leftServo.writeMicroseconds(1500);
    rightServo.writeMicroseconds(1500);
    darkRoom();
  }
  
  else if(mid > 30) {
    leftServo.writeMicroseconds(1750);
    rightServo.writeMicroseconds(1750);
  }

  else {
    pickSide();
  }

  
}

long microsecondsToInches(long microseconds) {
  // According to Parallax's datasheet for the PING))), there are 73.746
  // microseconds per inch (i.e. sound travels at 1130 feet per second).
  // This gives the distance travelled by the ping, outbound and return,
  // so we divide by 2 to get the distance of the obstacle.
  // See: http://www.parallax.com/dl/docs/prod/acc/28015-PING-v1.3.pdf
  return microseconds / 74 / 2;
}

void pickSide() {

  long durLeft, durRight, left, right;
  
  pinMode(pingLeft, OUTPUT);
  digitalWrite(pingLeft, LOW);
  delayMicroseconds(2);
  digitalWrite(pingLeft, HIGH);
  delayMicroseconds(5);
  digitalWrite(pingLeft, LOW);
  pinMode(pingLeft, INPUT);
  durLeft = pulseIn(pingLeft, HIGH);
  

  pinMode(pingRight, OUTPUT);
  digitalWrite(pingRight, LOW);
  delayMicroseconds(2);
  digitalWrite(pingRight, HIGH);
  delayMicroseconds(5);
  digitalWrite(pingRight, LOW);
  pinMode(pingRight, INPUT);
  durRight = pulseIn(pingRight, HIGH);

   delay(100);

  left = microsecondsToInches(durLeft);
  right = microsecondsToInches(durRight);

  Serial.print("Left: ");
  Serial.print(left);
  Serial.print(" in Right: ");
  Serial.print(right);
  Serial.print(" in\n");


  if(left > right) {
    for(i = 0; i < 100; i++) {
      leftServo.writeMicroseconds(1250);
      rightServo.writeMicroseconds(1750);
    }
  }
  else {
    for(i = 0; i < 100; i++) {
      rightServo.writeMicroseconds(1250);
      leftServo.writeMicroseconds(1750);
    }
  }

}

bool checkLight(){
  value = analogRead(pResistor);
  Serial.print(value);
  Serial.print("\n");
  //You can change value "25"
  if (value < 980){
    digitalWrite(ledPin, LOW);  //Turn led off
    return false;
  }
  else{
    digitalWrite(ledPin, HIGH); //Turn led on
    return true;
  }

  delay(500);
}

void darkRoom() {
  for(i = 0; i < 50; i++) {
    rightServo.writeMicroseconds(1200);
    leftServo.writeMicroseconds(1800);
    delay(10);
  }
  rightServo.writeMicroseconds(1500);
  leftServo.writeMicroseconds(1500);
  value = analogRead(darkResistor);
  Serial.print(value);
  Serial.print("\n");
  int newVal = value;
  while(value + 10 >= newVal){
    rightServo.writeMicroseconds(1700);
    leftServo.writeMicroseconds(1300);
    value = newVal;
    newVal = analogRead(darkResistor);
    Serial.print("New: ");
    Serial.print(newVal);
    Serial.print(" Old: ");
    Serial.print(value);
    Serial.print("\n");
  }
  rightServo.writeMicroseconds(1500);
  leftServo.writeMicroseconds(1500);
  moveDark();
  
}

void moveDark() {
  value = analogRead(darkResistor);
  int newVal = value;
  while(value + 10 >= newVal){
    rightServo.writeMicroseconds(1750);
    leftServo.writeMicroseconds(1750);
    value = newVal;
    newVal = analogRead(darkResistor);
    Serial.print("New: ");
    Serial.print(newVal);
    Serial.print(" Old: ");
    Serial.print(value);
    Serial.print("\n");
  }
  rightServo.writeMicroseconds(1500);
  leftServo.writeMicroseconds(1500);
  while(true) {
    rightServo.writeMicroseconds(1500);
    leftServo.writeMicroseconds(1500);
  }
  
}
