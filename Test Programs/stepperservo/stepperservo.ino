#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <AccelStepper.h>

// Define stepper motor connections and steps per revolution:
#define dirPin 2
#define stepPin 3
#define stepsPerRevolution 1000
#define stepperDelay 1000

// Constants for Servo Control
#define nbPCAServo 16
int MIN_IMP[nbPCAServo] = {500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500};
int MAX_IMP[nbPCAServo] = {2500, 2500, 2500, 2500, 2500, 2500, 2500, 2500, 2500, 2500, 2500, 2500, 2500, 2500, 2500, 2500};
int MIN_ANG[nbPCAServo] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int MAX_ANG[nbPCAServo] = {180, 180, 180, 180, 180, 180, 180, 180, 180, 180, 180, 180, 180, 180, 180, 180};

Adafruit_PWMServoDriver pca = Adafruit_PWMServoDriver(0x40);

void setup() {
  // Declare pins for the stepper motor as output:
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);

  // Init Serial USB
  Serial.begin(9600);
  Serial.println(F("Initialize System"));

  // Initialize I2C communication and PCA9685
  pca.begin();
  pca.setPWMFreq(50);  // Analog servos run at ~50 Hz update
}

void loop(){
  pcaScenario();  
  stepperMotorControl();
}

void pcaScenario(){/* function pcaScenario */ 
////Scenario to test servomotors controlled by PCA9685 I2C Module
for (int i=0; i<3; i++) {
  Serial.print("Servo");
  Serial.println(i);
      //int middleVal=((MAX_IMP[i]+MIN_IMP[i])/2)/20000*4096; // conversion Âµs to pwmval
      //pca.setPWM(i,0,middleVal);
      for(int pos=(MAX_IMP[i]+MIN_IMP[i])/2;pos<MAX_IMP[i];pos+=10){
        pca.writeMicroseconds(i,pos);
        pca.writeMicroseconds(i+1, pos);
        delayMicroseconds(1000);
      }
      for(int pos=MAX_IMP[i];pos>MIN_IMP[i];pos-=10){
        pca.writeMicroseconds(i,pos);
        pca.writeMicroseconds(i+1, pos);
        delayMicroseconds(1000);
      }
      for(int pos=MIN_IMP[i];pos<(MAX_IMP[i]+MIN_IMP[i])/2;pos+=10){
        pca.writeMicroseconds(i,pos);
        pca.writeMicroseconds(i+1, pos);
        delayMicroseconds(1000);
      }
      pca.setPin(i,0,true); // deactivate pin i
    }
}

void stepperMotorControl() {
  digitalWrite(dirPin, LOW);

  for (int i = 0; i < stepsPerRevolution; i++) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(stepperDelay);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(stepperDelay);
  }

  delayMicroseconds(stepperDelay);

  digitalWrite(dirPin, HIGH);

  for (int i = 0; i < stepsPerRevolution; i++) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(stepperDelay);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(stepperDelay);
  }
}