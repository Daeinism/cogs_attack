#include <Arduino.h>

#define MOTOR_LEFT_R_EN 19 //blue
#define MOTOR_LEFT_L_EN 18 //blue
#define MOTOR_LEFT_LPWM_FORWARD 22 //orange
#define MOTOR_LEFT_RPWM_BACKWARD 23 //yellow
#define MOTOR_RIGHT_R_EN 14//brown
#define MOTOR_RIGHT_L_EN 13//brown
#define MOTOR_RIGHT_RPWM_FORWARD 26 //orange
#define MOTOR_RIGHT_LPWM_BACKWARD 27 //yellow

#define CH1_PIN 2 //white
#define CH2_PIN 33 //green
#define CH4_PIN 32 //purple

#define DEADZONE 30

void setup() {
  Serial.begin(115200);

  pinMode(MOTOR_LEFT_R_EN, OUTPUT);
  pinMode(MOTOR_LEFT_L_EN, OUTPUT);
  pinMode(MOTOR_RIGHT_R_EN, OUTPUT);
  pinMode(MOTOR_RIGHT_L_EN, OUTPUT);

  //These are just an engine of the motor. Always ON
  digitalWrite(MOTOR_LEFT_R_EN, HIGH);
  digitalWrite(MOTOR_LEFT_L_EN, HIGH);
  digitalWrite(MOTOR_RIGHT_R_EN, HIGH);
  digitalWrite(MOTOR_RIGHT_L_EN, HIGH);

  //Signals from the joystick
  pinMode(CH1_PIN, INPUT);
  pinMode(CH2_PIN, INPUT);
  pinMode(CH4_PIN, INPUT);

  //Signals to motor drivers
  pinMode(MOTOR_LEFT_RPWM_BACKWARD, OUTPUT);
  pinMode(MOTOR_LEFT_LPWM_FORWARD, OUTPUT);
  pinMode(MOTOR_RIGHT_RPWM_FORWARD, OUTPUT);
  pinMode(MOTOR_RIGHT_LPWM_BACKWARD, OUTPUT);
}

void loop() {

  int ch1 = pulseIn(CH1_PIN, HIGH, 25000);   // CH1 PWM reading (1000~2000μs)
  int ch2 = pulseIn(CH2_PIN, HIGH, 25000);
  int ch4 = pulseIn(CH4_PIN, HIGH, 25000);

  int leftRight = map(ch1, 900, 2000, -255, 255);
  int forwardBack = map(ch2, 900, 2000, -255, 255);
  int FAN = map(ch4, 900, 2000, 0, 255);
  
  Serial.print("CH1: "); Serial.print(ch1);
  Serial.print(" LR: "); Serial.print(leftRight);
  Serial.print("\tCH2: "); Serial.print(ch2);
  Serial.print(" FB: "); Serial.print(forwardBack);
  Serial.print("\tCH4: "); Serial.println(ch4);

  // applying Deadzone
  if (abs(leftRight) < DEADZONE) leftRight = 0;

  if (forwardBack > DEADZONE) {
    analogWrite(MOTOR_LEFT_LPWM_FORWARD, abs(forwardBack));
    analogWrite(MOTOR_LEFT_RPWM_BACKWARD, 0);
    analogWrite(MOTOR_RIGHT_RPWM_FORWARD, abs(forwardBack));
    analogWrite(MOTOR_RIGHT_LPWM_BACKWARD, 0);
  }
  else if (forwardBack < -DEADZONE) {
    analogWrite(MOTOR_LEFT_LPWM_FORWARD, 0);
    analogWrite(MOTOR_LEFT_RPWM_BACKWARD, abs(forwardBack));
    analogWrite(MOTOR_RIGHT_RPWM_FORWARD, 0);
    analogWrite(MOTOR_RIGHT_LPWM_BACKWARD, abs(forwardBack));
  }
  else {
    analogWrite(MOTOR_LEFT_LPWM_FORWARD, 0);
    analogWrite(MOTOR_LEFT_RPWM_BACKWARD, 0);
    analogWrite(MOTOR_RIGHT_RPWM_FORWARD, 0);
    analogWrite(MOTOR_RIGHT_LPWM_BACKWARD, 0);
  }

  delay(20);

}


// void loop() {

//     for (int i = 0; i < MAX_SPEED; i += 10){
//     analogWrite(MOTOR_LEFT_LPWM_FORWARD, i);
//     analogWrite(MOTOR_RIGHT_RPWM_FORWARD, i);
//     analogWrite(MOTOR_RIGHT_LPWM_BACKWARD, 0);
//     analogWrite(MOTOR_LEFT_RPWM_BACKWARD, 0);
//     delay(250); 
//   }
//   for (int i = MAX_SPEED; i > 0; i -= 10){
//     analogWrite(MOTOR_LEFT_LPWM_FORWARD, i);
//     analogWrite(MOTOR_RIGHT_RPWM_FORWARD, i);
//     analogWrite(MOTOR_RIGHT_LPWM_BACKWARD, 0);
//     analogWrite(MOTOR_LEFT_RPWM_BACKWARD, 0);
//     delay(250); 
//   }
//   for (int i = 0; i < MAX_SPEED; i += 10){
//     analogWrite(MOTOR_LEFT_LPWM_FORWARD, 0);
//     analogWrite(MOTOR_RIGHT_RPWM_FORWARD, 0);
//     analogWrite(MOTOR_RIGHT_LPWM_BACKWARD, i);
//     analogWrite(MOTOR_LEFT_RPWM_BACKWARD, i);
//     delay(250); 
//   }
//   for (int i = MAX_SPEED; i > 0; i -= 10){
//     analogWrite(MOTOR_LEFT_LPWM_FORWARD, 0);
//     analogWrite(MOTOR_RIGHT_RPWM_FORWARD, 0);
//     analogWrite(MOTOR_RIGHT_LPWM_BACKWARD, i);
//     analogWrite(MOTOR_LEFT_RPWM_BACKWARD, i);
//     delay(250); 
//   }
  
// }

