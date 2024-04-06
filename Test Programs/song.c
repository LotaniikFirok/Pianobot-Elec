// Libraries
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <AccelStepper.h>

#define nbPCAServo 16

enum NOTE
{
  // the main ones
  C = 0,
  CS = 1,
  D = 2,
  DS = 3,
  E = 4,
  F = 5,
  FS = 6,
  G = 7,
  GS = 8,
  A = 9,
  AS = 10,
  B = 11,
  // alternative names
  BS = 0,
  DB = 1,
  EB = 3,
  FB = 4,
  ES = 5,
  GB = 6,
  AB = 8,
  BB = 10,
  CB = 11,
};

// Define stepper motor connections and steps per revolution:
#define dirPin 2
#define stepPin 3
#define motorInterfaceType 1
#define pulserate 1000
#define accelrate 10000
#define limit_switch 8

// Parameters
int MIN_IMP[nbPCAServo] = {500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500};
int MAX_IMP[nbPCAServo] = {2500, 2500, 2500, 2500, 2500, 2500, 2500, 2500, 2500, 2500, 2500, 2500, 2500, 2500, 2500, 2500};
int MIN_ANG[nbPCAServo] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int MAX_ANG[nbPCAServo] = {180, 180, 180, 180, 180, 180, 180, 180, 180, 180, 180, 180, 180, 180, 180, 180};

// LUT
int KEY_LUT[29] = {
    00, 01, 02, 03, 04,
    06, 07, 08, 09, 10, 11, 12,
    14, 15, 16, 17, 18,
    20, 21, 22, 23, 24, 25, 26,
    28, 29, 30, 31, 32}

// Objects
Adafruit_PWMServoDriver pca = Adafruit_PWMServoDriver(0x40);
AccelStepper stepper = AccelStepper(motorInterfaceType, stepPin, dirPin);

// Servo on periods
int down_pos = 2000;
int release_pos = 1000;

void setup()
{
  // Init Serial USB
  stepper.setMaxSpeed(pulserate);
  stepper.setAcceleration(accelrate);
  Serial.begin(9600);
  Serial.println(F("Initialize System"));
  pca.begin();
  pca.setPWMFreq(60); // Analog servos run at ~60 Hz updates

  pinMode(limit_switch, INPUT_PULLUP); // built in pull up, we forgot about it- Kai

  Serial.print("Initialization Complete");
}

void loop()
{
  Serial.print("Loop Begin");
  resetStepper();
  resetServo();
  pcaScenario();
}

void resetServo()
{
  // isolated the logic for resetting servo motors -Kai
  // resets position of servos
  Serial.print("Reseting Servo Motors");
  for (int i = 0; i < nbPCAServo; i++)
  {
    pca.writeMicroseconds(i, release_pos);
    delay(500);
    pca.setPin(i, 0, true);
  }
}

void resetStepper()
{
  // isolated the logic for homing stepper motor -Kai

  // reset main stepper motor
  Serial.print("Reseting Stepper Motors");
  stepper.setSpeed(-1000);
  stepper.moveTo(-4000); // this largest number of steps before hitting the end. calculated from 2 meter and 100 step per 6 cm.
  while (digitalRead(limit_switch) == HIGH)
  {
    stepper.run(); // non-blocking operation, no delay required.
  }
  stepper.setCurrentPosition(0);
}

void pcaScenario()
{ /* function pcaScenario */

  // This seems way too large, 6 cm for one note? -Kai
  int notesize = 100;
  int up_one_note = notesize;
  int down_one_note = -1 * notesize;
  int tempo = 200; // 69pbm/60/3(tripelets)*1000

  int triplet[3] = {0, 0, 0};

  // move to a position, bears no significance right now.
  delay(100);
  stepperMotorControl(9600); // isn't this way too long, like close to 5.76 meters? -Kai

  delay(1000);

  // bars 1-2 //Root note G - 1
  play_triplet(triplet, 2, 7, 10, tempo, 8);
  // bar 3 - 3.5
  play_triplet(triplet, 3, 7, 10, tempo, 2);
  Serial.print("3rd bar");
  // bar 3.5-4
  play_triplet(triplet, 3, 8, 12, tempo, 2);
  stepperMotorControl(down_one_note);

  // bar 4 //Root note F - 1
  play_triplet(triplet, 4, 8, 14, tempo, 1);
  play_triplet(triplet, 4, 9, 12, tempo, 1);
  play_triplet(triplet, 4, 9, 11, tempo, 1);
  play_triplet(triplet, 2, 7, 11, tempo, 1);
  stepperMotorControl(down_one_note);

  // bar 5 //Root note E - 1
  play_triplet(triplet, 1, 5, 10, tempo, 1);
  stepperMotorControl(2 * up_one_note);

  // Root G
  play_triplet(triplet, 2, 7, 10, tempo, 2);
  holdkey(14);
  play_triplet(triplet, 2, 7, 10, tempo, 1);
  release(14);
  presskey(14, 50);

  // bar 6 root G
  holdkey(14);
  play_triplet(triplet, 2, 9, 12, tempo, 3);
  release(14);

  holdkey(14);
  play_triplet(triplet, 2, 9, 12, tempo, 1);
  release(14);
  presskey(14, 50);

  // bar 7 root G
  holdkey(14);
  play_triplet(triplet, 2, 7, 10, tempo, 2);
  release(14);
  stepperMotorControl(up_one_note);

  // root A
  holdkey(13);
  play_triplet(triplet, 1, 5, 8, tempo, 2);
  release(13);

  stepperMotorControl(down_one_note);

  // bar 8 root G
  holdkey(14);
  play_triplet(triplet, 2, 5, 10, tempo, 2);
  release(14);

  stepperMotorControl(up_one_note);
  // root A
  holdkey(10);
  play_triplet(triplet, 1, 3, 7, tempo, 1);
  release(10);
  stepperMotorControl(up_one_note);

  // root B
  play_triplet(triplet, 13, 1, 5, tempo, 1);
  stepperMotorControl(2 * down_one_note);

  // bar 9 root G
  holdkey(10);
  play_triplet(triplet, 2, 5, 10, tempo, 1);
  release(10);
  play_triplet(triplet, 2, 5, 10, tempo, 3);

  // bar 10 root G
  play_triplet(triplet, 1, 5, 10, tempo, 3);
  holdkey(13);
  play_triplet(triplet, 1, 5, 10, tempo, 1);
  release(13);

  // bar 11 root G
  holdkey(13);
  play_triplet(triplet, 1, 5, 11, tempo, 3);
  release(13);

  holdkey(13);
  play_triplet(triplet, 1, 5, 11, tempo, 1);
  release(13);
  presskey(13, 50);

  // Bar 12 root G
  holdkey(13);
  play_triplet(triplet, 1, 6, 9, tempo, 1);
  play_triplet(triplet, 1, 5, 9, tempo, 1);
  play_triplet(triplet, 1, 7, 9, tempo, 1);
  release(13);
  stepperMotorControl(down_one_note);
  // Root F
  holdkey(14);
  play_triplet(triplet, 2, 9, 12, tempo, 1);
  release(14);

  // Bar 13 root F
  holdkey(14);
  play_triplet(triplet, 2, 7, 10, tempo, 2);
  release(14);
  stepperMotorControl(up_one_note);
  // root G
  holdkey(13);
  play_triplet(triplet, 1, 5, 7, tempo, 1);
  release(13);
  stepperMotorControl(2 * down_one_note);
  // root E
  holdkey(13);
  play_triplet(triplet, 1, 8, 10, tempo, 1);
  release(13);

  stepperMotorControl(up_one_note);

  // Bar 14 root F
  holdkey(14);
  play_triplet(triplet, 2, 7, 10, tempo, 2);
  release(14);
  holdkey(14);
  play_triplet(triplet, 2, 6, 9, tempo, 2);
  release(14);

  stepperMotorControl(3 * up_one_note);

  // Bar 15 root B
  play_triplet(triplet, 1, 4, 8, tempo, 2);
  play_triplet(triplet, 1, 5, 8, tempo, 1);
  holdkey(13);
  play_triplet(triplet, 1, 5, 8, tempo, 1);
  release(13);
  stepperMotorControl(2 * down_one_note);

  // Complete
  delay(5000);

  // //Bar 16 root (C actually B)
  // stepperMotorControl(-1*notesize);
  // play_triplet(triplet,13,5,8,tempo,1);
}

/// @brief Move stepper motor by a number of steps relative to the current position and reset the coordinate system.
/// @param targetPosition The number of steps to move by.
void stepperMotorControl(int targetPosition)
{
  // Set the target position for the stepper motor
  stepper.moveTo(targetPosition);

  // Run the stepper motor until it reaches the target position
  stepper.runToPosition(); // blocking operation
  stepper.setCurrentPosition(0);
  delay(100);

  // I question this approach of resetting the position maybe use the following instead.
  // this way, homing information is not lost.- Kai
  // stepper.moveTo(stepper.currentPosition()+targetPosition);
  // stepper.runToPosition();       // blocking operation
}

void release(int key)
{
  pca.writeMicroseconds(key, release_pos);
  delay(100);
  pca.setPin(key, 0, true);
}

void holdkey(int key)
{
  pca.writeMicroseconds(key, down_pos);
  delay(100);
  pca.setPin(key, 0, true);
}

void presskey(int key, int time)
{
  pca.writeMicroseconds(key, down_pos);
  delay(time);
  release(key);
}

void play_triplet(int *triple, int a, int b, int c, int time, int loops)
{
  triple[0] = a;
  triple[1] = b;
  triple[2] = c;
  for (int i = 0; i < loops; i++)
  {
    for (int i = 0; i < 3; i++)
    {
      presskey(triple[i], time);
    }
  }
}

/// @brief Press a Note
/// @param base_note The note that the left most finger correspond to.
/// @param desired_note The note that wish to be pressed.
/// @param time  The duration of the note.
/// Use this if you like, maybe even tracks the current base note globally.
void pressNote(Note base_note, Note desired_note, int time)
{
  presskey(mapNoteToServo(base_note, desired_note), time);
}

/// @brief Map a Note to the Corresponding Servo.
/// @param base_note The note that the left most finger correspond to.
/// @param desired_note The note that wish to be pressed.
/// @return The servo that would press `desired_note`
/// Use this if you like, maybe even tracks the current base note globally.
int mapNoteToServo(NOTE base_note, NOTE desired_note)
{
  // get the next integer that represent the desired note in the octave that follows the current base_note.
  int corrected_desired_note = ((int)desired_note - (int)base_note + 12) % 12;
  return KEY_LUT[(int)base_note] - KEY_LUT[(int)corrected_desired_note];
}