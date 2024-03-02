//Libraries
#include <Wire.h>//https://www.arduino.cc/en/reference/wire
#include <Adafruit_PWMServoDriver.h>//https://github.com/adafruit/Adafruit-PWM-Servo-Driver-Library
//Constants
#define nbPCAServo 16
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
////Scenario to test servomotors controlled by PCA9685 I2C Module

  int tempo = 383; //69pbm/60/3(tripelets)*1000
  int[3] triplet = {0,0,0}
  for (int i=0; i<nbPCAServo; i++) {
    //bars 1-2
    play_triplet(triplet,2,7,10,tempo,8);
    //bar 3 - 3.5
    play_triplet(triplet,3,7,10,tempo,2);
    //bar 3.5-4
    play_triplet(triplet,3,8,12,tempo,2);
    //bar 4
    play_triplet(triplet,3,8,12,tempo,2);
}

int jointToImp(double x,int i){/* function jointToImp */
////Convert joint angle into pwm command value
      int imp=(x - MIN_ANG[i]) * (MAX_IMP[i]-MIN_IMP[i]) / (MAX_ANG[i]-MIN_ANG[i]) + MIN_IMP[i];
      imp=max(imp,MIN_IMP[i]);
      imp=min(imp,MAX_IMP[i]);
 return imp;
}

void presskey(int key, int time){
    pca.writeMicroseconds(key,1000);
    delay(time);
    pca.writeMicroseconds(key,1500);
  
}
void play_triplet(int *triplet,int a,int b, int c, int time,int loops){
  triple[0] = a;
  triple[1] = b;
  triple[2] = c;
  for(int i = 0;i<loops;++){
    for(int i = 0;i<3;i++){
      presskey(triplet[i],time);
    }
  }
}

