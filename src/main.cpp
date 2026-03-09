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
#define ALPHA 0.3 //reaction speed (Min 0.1 (10%) ~ Max 1.0 (100%))
#define STEERING_GAIN 0.6 //dampening the cornering speed (Min 0.1 (10%) ~ Max 1.0 (100%))

float filteredForwardBack = 0.0;
float filteredLeftRight = 0.0;


/*|Functions|----------------------------------------------------------------*/
void setLeftMotor(int pwm) {
  pwm = constrain(pwm, -255, 255);

  if (pwm > 0) {
    analogWrite(MOTOR_LEFT_LPWM_FORWARD, pwm);
    analogWrite(MOTOR_LEFT_RPWM_BACKWARD, 0);
  } 
  else if (pwm < 0) {
    analogWrite(MOTOR_LEFT_LPWM_FORWARD, 0);
    analogWrite(MOTOR_LEFT_RPWM_BACKWARD, -pwm);
  } 
  else {
    analogWrite(MOTOR_LEFT_LPWM_FORWARD, 0);
    analogWrite(MOTOR_LEFT_RPWM_BACKWARD, 0);
  }
}

void setRightMotor(int pwm) {
  pwm = constrain(pwm, -255, 255);

  if (pwm > 0) {
    analogWrite(MOTOR_RIGHT_RPWM_FORWARD, pwm);
    analogWrite(MOTOR_RIGHT_LPWM_BACKWARD, 0);
  } 
  else if (pwm < 0) {
    analogWrite(MOTOR_RIGHT_RPWM_FORWARD, 0);
    analogWrite(MOTOR_RIGHT_LPWM_BACKWARD, -pwm);
  } 
  else {
    analogWrite(MOTOR_RIGHT_RPWM_FORWARD, 0);
    analogWrite(MOTOR_RIGHT_LPWM_BACKWARD, 0);
  }
}

void serialPrint (int ch1, int ch2, int ch4, int leftRight, int forwardBack){
  Serial.print("CH1: "); Serial.print(ch1);
  Serial.print(" LR: "); Serial.print(leftRight);
  Serial.print("   CH2: "); Serial.print(ch2);
  Serial.print(" FB: "); Serial.print(forwardBack);
  Serial.print("   CH4: "); Serial.println(ch4);
}

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

  analogWriteFrequency(20000); //To get rid of annoying noise

}

void loop() {

  int ch1 = pulseIn(CH1_PIN, HIGH, 25000);   // CH1 PWM reading (1000~2000μs)
  int ch2 = pulseIn(CH2_PIN, HIGH, 25000);
  int ch4 = pulseIn(CH4_PIN, HIGH, 25000);

  int leftRight   = map(ch1, 900, 2000, -255, 255);
  int forwardBack = map(ch2, 900, 2000, -255, 255);
  int FAN         = map(ch4, 900, 2000, 0, 255);
  
  serialPrint (ch1, ch2, ch4, leftRight, forwardBack);

  // failSafe system
  if (ch1 == 0 || ch2 == 0) {
    setLeftMotor(0);
    setRightMotor(0);
    return;
  }

  // applying Deadzone
  if (abs(leftRight) < DEADZONE) leftRight = 0;
  if (abs(forwardBack) < DEADZONE) forwardBack = 0;



  // Speed smoothing filter
  filteredForwardBack = forwardBack * ALPHA + filteredForwardBack * (1.0 - ALPHA);
  filteredLeftRight   = leftRight   * ALPHA + filteredLeftRight   * (1.0 - ALPHA);

  int leftMotor  = (int)(filteredForwardBack + filteredLeftRight * STEERING_GAIN);
  int rightMotor = (int)(filteredForwardBack - filteredLeftRight * STEERING_GAIN);

  setLeftMotor(leftMotor);
  setRightMotor(rightMotor);

  delay(20); //loop is set to 50 Hz 
  /*
  50ms = 20Hz   Slow
  20ms = 50Hz   Standard rangefor RC/servo system 
  10ms = 100Hz  Fast and smooth
  5ms  = 200Hz  Too much
  */

}