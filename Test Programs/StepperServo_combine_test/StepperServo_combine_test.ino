//Libraries
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <AccelStepper.h>

#define nbPCAServo 16


// Define stepper motor connections and steps per revolution:
#define dirPin 2
#define stepPin 3
#define stepsPerRevolution 1000
#define stepperDelay 1000

//Parameters
int MIN_IMP [nbPCAServo] ={500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500};
int MAX_IMP [nbPCAServo] ={2500, 2500, 2500, 2500, 2500, 2500, 2500, 2500, 2500, 2500, 2500, 2500, 2500, 2500, 2500, 2500};
int MIN_ANG [nbPCAServo] ={0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int MAX_ANG [nbPCAServo] ={180, 180, 180, 180, 180, 180, 180, 180, 180, 180, 180, 180, 180, 180, 180, 180};
//Objects
Adafruit_PWMServoDriver pca= Adafruit_PWMServoDriver(0x40);


void setup(){
//Init Serial USB
  Serial.begin(9600);
  Serial.println(F("Initialize System"));
  pca.begin();
  pca.setPWMFreq(60);  // Analog servos run at ~60 Hz updates
}
void loop(){
  pcaScenario();
}
void pcaScenario(){/* function pcaScenario */

  int tempo = 100; //69pbm/60/3(tripelets)*1000
  Serial.print("starting...");
  int triplet[3] = {0,0,0};
  for(int i =0;i<nbPCAServo;i++){
    pca.setPin(i,0,true);
  }
  delay(1000);
  for(int i =0;i<nbPCAServo;i++){
    release(i);
  }
  Serial.print("reset");
  //for (int i=0; i<nbPCAServo; i++) {
    //bars 1-2 //Root note G - 1
    play_triplet(triplet,2,7,10,tempo,8);
    Serial.print("test");
    //bar 3 - 3.5 
    play_triplet(triplet,3,7,10,tempo,2);
    Serial.print("3rd bar");
    //bar 3.5-4
    play_triplet(triplet,3,8,12,tempo,2);
    //stepperMotorControl(-100);
    //bar 4 //Root note F - 1
    play_triplet(triplet,4,8,14,tempo,1);
    play_triplet(triplet,4,9,12,tempo,1);
    play_triplet(triplet,4,9,11,tempo,1);
    play_triplet(triplet,2,7,11,tempo,1);
    //stepperMotorControl(-100);

    //bar 5 //Root note E - 1
    play_triplet(triplet,1,5,10,tempo,1);
    // Root G
    play_triplet(triplet,2,7,10,tempo,2);
    pca.writeMicroseconds(14,2000);
    play_triplet(triplet,2,7,10,tempo,1);
    release(14);
    presskey(14,50);

    //bar 6 root G
    pca.writeMicroseconds(14,2000);
    play_triplet(triplet,2,9,12,tempo,3);
    release(14);
    play_triplet(triplet,2,9,12,tempo,1);
    presskey(14,50);
    //bar 7 root G
    pca.writeMicroseconds(14,2000);
    play_triplet(triplet,2,7,10,tempo,2);
    release(14);
    //root A
    pca.writeMicroseconds(13,2000);
    play_triplet(triplet,1,5,8,tempo,2);
    release(13);   
    // bar 8 root G
    pca.writeMicroseconds(14,2000);
    play_triplet(triplet,2,5,10,tempo,2);
    release(14);
    // root A
    pca.writeMicroseconds(10,2000);
    play_triplet(triplet,1,3,7,tempo,1);
    release(10);
    // root B
    play_triplet(triplet,13,1,5,tempo,1);

    //bar 9 root G
    pca.writeMicroseconds(10,2000);
    play_triplet(triplet,2,5,10,tempo,1);
    release(10);
    play_triplet(triplet,2,5,10,tempo,3);

    //bar 10 root G
    play_triplet(triplet,1,5,10,tempo,3);
    pca.writeMicroseconds(13,2000);
    play_triplet(triplet,1,5,10,tempo,1);
    release(13);

}

// void stepperMotorControl(int targetPosition) {
//   // Set the target position for the stepper motor
//   stepper.moveTo(targetPosition);

//   // Run the stepper motor until it reaches the target position
//   while (stepper.isRunning()) {
//     stepper.run();
//     delay(1);
//   }
// }

void release(int key){
  pca.writeMicroseconds(key,1500);
  delay(100);
  pca.setPin(key,0,true);
}

void presskey(int key, int time){
    pca.writeMicroseconds(key,2000);
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

