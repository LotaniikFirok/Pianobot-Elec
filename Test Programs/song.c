//Libraries
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <AccelStepper.h>

#define nbPCAServo 16


// Define stepper motor connections and steps per revolution:
#define dirPin 2
#define stepPin 3
#define motorInterfaceType 1
#define pulserate 1000
#define accelrate 10000



//Parameters
int MIN_IMP [nbPCAServo] ={500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500};
int MAX_IMP [nbPCAServo] ={2500, 2500, 2500, 2500, 2500, 2500, 2500, 2500, 2500, 2500, 2500, 2500, 2500, 2500, 2500, 2500};
int MIN_ANG [nbPCAServo] ={0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int MAX_ANG [nbPCAServo] ={180, 180, 180, 180, 180, 180, 180, 180, 180, 180, 180, 180, 180, 180, 180, 180};
//Objects
Adafruit_PWMServoDriver pca= Adafruit_PWMServoDriver(0x40);

AccelStepper stepper = AccelStepper(motorInterfaceType, stepPin, dirPin);

int down_pos = 2200;
int release_pos = 1500;

void setup(){
//Init Serial USB
  stepper.setMaxSpeed(pulserate);
  stepper.setAcceleration(accelrate);
  Serial.begin(9600);
  Serial.println(F("Initialize System"));
  pca.begin();
  pca.setPWMFreq(60);  // Analog servos run at ~60 Hz updates
}
void loop(){
  pcaScenario();
}
void pcaScenario(){/* function pcaScenario */

  int notesize = 100;
  int uponesemi = notesize;
  int down_one_note = -1*notesize;
  int tempo = 200; //69pbm/60/3(tripelets)*1000
  Serial.print("starting...");
  int triplet[3] = {0,0,0};

  
  // pca.setPin(3,0,true);
  // delay(1000);
  // release(3);
  // stepperMotorControl(-1*notesize);
  // presskey(3,1000);
  // stepperMotorControl(notesize);


//resets position of servos

  Serial.print("reseting motors");
  for(int i =0;i<nbPCAServo;i++){
    pca.writeMicroseconds(i,release_pos);
    delay(500);
    pca.setPin(i,0,true);
  }
  Serial.print("reset done");
  delay(5000);

    
    //bars 1-2 //Root note G - 1
    play_triplet(triplet,2,7,10,tempo,8);
    //bar 3 - 3.5 
    play_triplet(triplet,3,7,10,tempo,2);
    Serial.print("3rd bar");
    //bar 3.5-4
    play_triplet(triplet,3,8,12,tempo,2);
    stepperMotorControl(down_one_note);


    //bar 4 //Root note F - 1
    play_triplet(triplet,4,8,14,tempo,1);
    play_triplet(triplet,4,9,12,tempo,1);
    play_triplet(triplet,4,9,11,tempo,1);
    play_triplet(triplet,2,7,11,tempo,1);
    stepperMotorControl(down_one_note);

    //bar 5 //Root note E - 1
    play_triplet(triplet,1,5,10,tempo,1);
    stepperMotorControl(2*notesize);

    // Root G
    play_triplet(triplet,2,7,10,tempo,2);
    holdkey(14);
    play_triplet(triplet,2,7,10,tempo,1);
    release(14);
    presskey(14,50);

    //bar 6 root G
    holdkey(14);
    play_triplet(triplet,2,9,12,tempo,3);
    release(14);

    holdkey(14);
    play_triplet(triplet,2,9,12,tempo,1);
    release(14);
    presskey(14,50);

    //bar 7 root G
    holdkey(14);
    play_triplet(triplet,2,7,10,tempo,2);
    release(14);
    stepperMotorControl(notesize);

    //root A
    holdkey(13);
    play_triplet(triplet,1,5,8,tempo,2);
    release(13);   

    stepperMotorControl(down_one_note);

    // bar 8 root G
    holdkey(14);
    play_triplet(triplet,2,5,10,tempo,2);
    release(14);

    stepperMotorControl(notesize);
    // root A
    holdkey(10);
    play_triplet(triplet,1,3,7,tempo,1);
    release(10);
    stepperMotorControl(notesize);

    // root B
    play_triplet(triplet,13,1,5,tempo,1);
    stepperMotorControl(2*down_one_note);

    //bar 9 root G
    holdkey(10);
    play_triplet(triplet,2,5,10,tempo,1);
    release(10);
    play_triplet(triplet,2,5,10,tempo,3);

    //bar 10 root G
    play_triplet(triplet,1,5,10,tempo,3);
    holdkey(13);
    play_triplet(triplet,1,5,10,tempo,1);
    release(13);

    // bar 11 root G
    holdkey(13);
    play_triplet(triplet,1,5,11,tempo,3);
    release(13);

    holdkey(13);
    play_triplet(triplet,1,5,11,tempo,1);
    release(13);
    presskey(13,50);

    // Bar 12 root G
    holdkey(13);
    play_triplet(triplet,1,6,9,tempo,1);
    play_triplet(triplet,1,5,9,tempo,1);
    play_triplet(triplet,1,7,9,tempo,1);
    release(13);
    stepperMotorControl(down_one_note);
    // Root F
    holdkey(14);
    play_triplet(triplet,2,9,12,tempo,1);
    release(14);

    // Bar 13 root F
    holdkey(14);
    play_triplet(triplet,2,7,10,tempo,2);
    release(14);
    stepperMotorControl(notesize);
    // root G
    holdkey(13);
    play_triplet(triplet,1,5,7,tempo,1);
    release(13);
    stepperMotorControl(-2*notesize);
    // root E
    holdkey(13);
    play_triplet(triplet,1,8,10,tempo,1);
    release(13);

    stepperMotorControl(2*notesize);
}

void stepperMotorControl(int targetPosition) {
  // Set the target position for the stepper motor
  stepper.moveTo(targetPosition);
  
  // Run the stepper motor until it reaches the target position
  stepper.runToPosition();
  stepper.setCurrentPosition(0);
  delay(100);

}

void release(int key){
  pca.writeMicroseconds(key,release_pos);
  delay(100);
  pca.setPin(key,0,true);
}

void holdkey(int key){
  pca.writeMicroseconds(key,down_pos);
  delay(100);
  pca.setPin(key,0,true);
}

void presskey(int key, int time){
    pca.writeMicroseconds(key,down_pos);
    delay(time);
    release(key);
  
}

void play_triplet(int *triple,int a,int b, int c, int time,int loops){
  triple[0] = a;
  triple[1] = b;
  triple[2] = c;
  for(int i = 0;i<loops;i++){
    for(int i = 0;i<3;i++){
      presskey(triple[i],time);
    }
  }
}


