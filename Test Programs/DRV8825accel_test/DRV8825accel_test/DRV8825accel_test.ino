#include <AccelStepper.h>

#define dirPin 2
#define stepPin 3
#define motorInterfaceType 1
#define pulserate 1000
#define accelrate 150

AccelStepper stepper = AccelStepper(motorInterfaceType, stepPin, dirPin);

void setup() {
  // maximum speed and acceleration:
  stepper.setMaxSpeed(pulserate);
  stepper.setAcceleration(accelrate);
}

void loop() {
  // target position:
  stepper.moveTo(3000);
  // Run to target position with set speed and acceleration/deceleration:
  stepper.runToPosition();

  delay(1000);

  // Move back to zero:
  stepper.moveTo(0);
  stepper.runToPosition();

  delay(1000);
}