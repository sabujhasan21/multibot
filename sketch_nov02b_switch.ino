#include <AFMotor.h> // motor liberary
#include <Servo.h>  //servo liberary

AF_DCMotor m1 = 1;  //motor1 object declear
AF_DCMotor m2 = 2;  //motor2 object declear
AF_DCMotor m3 = 3;  //motor3 object declear
AF_DCMotor m4 = 4;  //motor4 object declear

Servo servo;  //servo object declear

#define left_ir A0  //define the left side IR pin
#define right_ir A1 //define the right side IR pin
#define trig A4     //define the trigger pin
#define echo A2     //define the echo pin

char sms;           //charecter sms
//int led = A2;       //led pin
long duration;      //long type magnitude 
int distance;       //for distance
int distance_L, distance_F, distance_R; //distance form left right and forward

void setup() {
  Serial.begin(9600);       //serial comunication
  //pinMode(led, OUTPUT);     //led as output
  pinMode(left_ir, INPUT);  //characterstic of ir pin
  pinMode(right_ir, INPUT); //characterstic of ir pin
  pinMode(trig, OUTPUT);    //characterstic of trig pin
  pinMode(echo, INPUT);     //characterstic of echo pin
  servo.attach(9);          //servo pin declear
}

void loop() {
  if(Serial.available() > 0){ //check any signal is comming?
    sms = Serial.read(); //signal converst as sms
  }
    switch(sms){
      case 'f':
        forward();
        break;
      case 'b':
        backward();
        break;
      case 'l':
        left();
        break;
      case 'r':
        right();
        break;
      case 's':
        off();
        break;
      case 'i':
        ir_bot();
        break;
      case 'u':
        ultrasound();
        Serial.println(distance);
        break;
    }
 
}

void forward(){
  Serial.println("forward");
  m1.run(FORWARD);
  m1.setSpeed(110);
  m2.run(FORWARD);
  m2.setSpeed(110);
  m3.run(FORWARD);
  m3.setSpeed(110);
  m4.run(FORWARD);
  m4.setSpeed(110);
}
void backward(){
  Serial.println("backward");
  m1.run(BACKWARD);
  m1.setSpeed(110);
  m2.run(BACKWARD);
  m2.setSpeed(110);
  m3.run(BACKWARD);
  m3.setSpeed(110);
  m4.run(BACKWARD);
  m4.setSpeed(110); 
}
void left(){
  Serial.println("left");
  m1.run(BACKWARD);
  m1.setSpeed(110);
  m2.run(BACKWARD);
  m2.setSpeed(110);
  m3.run(FORWARD);
  m3.setSpeed(110);
  m4.run(FORWARD);
  m4.setSpeed(110); 
}
void right(){
  Serial.println("right");
  m1.run(FORWARD);
  m1.setSpeed(110);
  m2.run(FORWARD);
  m2.setSpeed(110);
  m3.run(BACKWARD);
  m3.setSpeed(110);
  m4.run(BACKWARD);
  m4.setSpeed(110);
}
void off(){
  Serial.println("off");
  m1.run(RELEASE);
  m2.run(RELEASE);
  m3 .run(RELEASE);
  m4.run(RELEASE);
}
//line follow mode
void ir_bot(){
  if(digitalRead(left_ir) == 0 && digitalRead(right_ir) == 0){
    forward();
  }
  //line detected by left ir
  if(digitalRead(left_ir) == 1 && digitalRead(right_ir) == 0){
    left();
  }
  //line detected by right ir
  if(digitalRead(left_ir) == 0 && digitalRead(right_ir) == 1){
    right();
  }
  //line detected by both ir
  if(digitalRead(left_ir) == 1 && digitalRead(right_ir) == 1){
   off();
  }
}

//obstacle avoiding mode
void ultrasound(){
  distance_F = data();
  if(distance_F > 40){
    forward();
  }
  else{
    sensorRead();
  }
}
long data(){
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);

  duration = pulseIn(echo, HIGH);
  distance = duration * 0.0344/2;
  return distance;
}
void sensorRead(){
  off();
  servo.write(0);
  delay(300);
  distance_L = data();
  delay(200);
  
  servo.write(180);
  delay(300);
  distance_R = data();
  delay(200);

  servo.write(90);
  delay(300);
  dataCheck();
}
void dataCheck(){
  if(distance_L > distance_R){
    left();
    delay(500);
  }
  if(distance_L < distance_R){
    right();
    delay(500);
  }
  else{
    backward();
    delay(500);
    right();
  }
}
