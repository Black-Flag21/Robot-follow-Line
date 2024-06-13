#include <NewPing.h>
#include <Servo.h>
#include <AFMotor.h>

// HC-SR04 sensor
#define TRIGGER_PIN A2
#define ECHO_PIN A3
#define MAX_DISTANCE 50

// IR sensor
#define IR_LEFT A0
#define IR_RIGHT A1

// Motor
#define MAX_SPEED 200
#define MAX_SPEED_OFFSET 20

Servo servo;

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

AF_DCMotor motor1(1, MOTOR12_1KHZ);
AF_DCMotor motor2(2, MOTOR12_1KHZ);
AF_DCMotor motor3(3, MOTOR34_1KHZ);
AF_DCMotor motor4(4, MOTOR34_1KHZ);

int distance = 0;
int leftDistance;
int rightDistance;
boolean object;

void setup() {
  Serial.begin(9600);
  pinMode(IR_LEFT, INPUT);
  pinMode(IR_RIGHT, INPUT);
  servo.attach(10);
  servo.write(90);

  setMotorSpeed(120);
}

void loop() {
  int irLeft = digitalRead(IR_LEFT);
  int irRight = digitalRead(IR_RIGHT);

  if (irLeft == 0 && irRight == 0) {
    objectAvoid();
    moveForward();
  } else if (irLeft == 0 && irRight == 1) {
    objectAvoid();
    Serial.println("TL");
    moveLeft();
  } else if (irLeft == 1 && irRight == 0) {
    objectAvoid();
    Serial.println("TR");
    moveRight();
  } else {
    Stop();
  }
}

void objectAvoid() {
  distance = getDistance();
  if (distance <= 20) {
    Stop();
    Serial.println("Stop");

    lookLeft();
    lookRight();
    delay(100);
    if (rightDistance <= leftDistance) {
      object = true;
      turn();
      Serial.println("moveLeft");
    } else {
      object = false;
      turn();
      Serial.println("moveRight");
    }
    delay(100);
  } else {
    Serial.println("moveForward");
    moveForward();
  }
}

int getDistance() {
  delay(50);
  int cm = sonar.ping_cm();
  return cm == 0 ? 100 : cm;
}

void lookLeft() {
  servo.write(150);
  delay(500);
  leftDistance = getDistance();
  delay(100);
  servo.write(90);
  Serial.print("Left:");
  Serial.print(leftDistance);
}

void lookRight() {
  servo.write(30);
  delay(500);
  rightDistance = getDistance();
  delay(100);
  servo.write(90);
  Serial.print("   Right:");
  Serial.println(rightDistance);
}

void Stop() {
  setMotorState(RELEASE);
}

void moveForward() {
  setMotorState(FORWARD);
}

void moveBackward() {
  setMotorState(BACKWARD);
}

void turn() {
  if (object == false) {
    Serial.println("turn Right");
    moveLeft();
    delay(700);
    moveForward();
    delay(800);
    moveRight();
    delay(500);
    moveForward();
    delay(1000);
    moveRight();
    delay(700);
  } else {
    Serial.println("turn Left");
    moveRight();
    delay(700);
    moveForward();
    delay(800);
    moveLeft();
    delay(500);
    moveForward();
    delay(1000);
    moveLeft();
    delay(700);
  }
  moveForward();
}

void moveRight() {
  motor1.run(BACKWARD);
  motor2.run(BACKWARD);
  motor3.run(FORWARD);
  motor4.run(FORWARD);
}

void moveLeft() {
  motor1.run(FORWARD);
  motor2.run(FORWARD);
  motor3.run(BACKWARD);
  motor4.run(BACKWARD);
}

void setMotorSpeed(int speed) {
  motor1.setSpeed(speed);
  motor2.setSpeed(speed);
  motor3.setSpeed(speed);
  motor4.setSpeed(speed);
}

void setMotorState(int state) {
  motor1.run(state);
  motor2.run(state);
  motor3.run(state);
  motor4.run(state);
}
