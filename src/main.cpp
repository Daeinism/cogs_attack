#include <Arduino.h>

#define MOTOR_LEFT_R_EN 19 //blue
#define MOTOR_LEFT_L_EN 18 //blue
#define MOTOR_LEFT_RPWM 23 //orange
#define MOTOR_LEFT_LPWM 22 //yellow
#define MOTOR_RIGHT_R_EN 14//brown
#define MOTOR_RIGHT_L_EN 13//brown
#define MOTOR_RIGHT_RPWM 26 //orange
#define MOTOR_RIGHT_LPWM 27 //yellow

#define CH1_PIN 35 //white
#define CH2_PIN 34 //green
#define CH3_PIN 33 //purple

void setup() {
  Serial.begin(115200);

  //These are just an engine of the motor. Always ON
  digitalWrite(MOTOR_LEFT_R_EN, HIGH);
  digitalWrite(MOTOR_LEFT_L_EN, HIGH);
  digitalWrite(MOTOR_RIGHT_R_EN, HIGH);
  digitalWrite(MOTOR_RIGHT_L_EN, HIGH);

  //Signals from the joystick
  pinMode(CH1_PIN, INPUT);
  pinMode(CH2_PIN, INPUT);
  pinMode(CH3_PIN, INPUT);

  //Signals to motor drivers
  pinMode(MOTOR_LEFT_RPWM, OUTPUT);
  pinMode(MOTOR_LEFT_LPWM, OUTPUT);
  pinMode(MOTOR_RIGHT_RPWM, OUTPUT);
  pinMode(MOTOR_RIGHT_LPWM, OUTPUT);
}

void loop() {
  for (int i = 0; i < 255; i += 10){
    analogWrite(MOTOR_LEFT_RPWM, i);
    analogWrite(MOTOR_LEFT_RPWM, 0);
    delay(250); 
  }
  for (int i = 250; i > 0; i -= 10){
    analogWrite(MOTOR_LEFT_RPWM, i);
    analogWrite(MOTOR_LEFT_RPWM, 0);
    delay(250); 
  }
    for (int i = 0; i < 255; i += 10){
    analogWrite(MOTOR_LEFT_LPWM, i);
    analogWrite(MOTOR_LEFT_LPWM, 0);
    delay(250); 
  }
  for (int i = 250; i > 0; i -= 10){
    analogWrite(MOTOR_LEFT_LPWM, i);
    analogWrite(MOTOR_LEFT_LPWM, 0);
    delay(250); 
  }
}
