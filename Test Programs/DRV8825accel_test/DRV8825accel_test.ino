#include <AccelStepper.h>

#define dirPin 2
#define stepPin 3
#define motorInterfaceType 1
#define pulserate 1000
#define accelrate 10000

AccelStepper stepper = AccelStepper(motorInterfaceType, stepPin, dirPin);

void setup() {
  // maximum speed and acceleration:
  stepper.setMaxSpeed(pulserate);
  stepper.setAcceleration(accelrate);
}

void loop() {
  Serial.print("start");
  // target position:
  stepper.moveTo(200);
  Serial.print("moving to 3000");
  // Run to target position with set speed and acceleration/deceleration:
  stepper.runToPosition();
  Serial.print("done");
  delay(2000);

  // Move back to zero:
  Serial.print("moving back to 0");
  stepper.moveTo(0);
  stepper.runToPosition();

  delay(2000);
}
