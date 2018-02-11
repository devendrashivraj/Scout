#include <SparkFun_TB6612.h>
#include <Wire.h>

#define AIN1 2
#define BIN1 7
#define AIN2 4
#define BIN2 8
#define PWMA 5
#define PWMB 6
#define STBY 9

const int offsetA = -1;
const int offsetB = -1;
int val, reading;
unsigned long startTime;

Motor motor1 = Motor(AIN1, AIN2, PWMA, offsetA, STBY);
Motor motor2 = Motor(BIN1, BIN2, PWMB, offsetB, STBY);

void setup() {
  Wire.begin(42);
  Wire.onReceive(receiveCommands);

  Serial.begin(9600);
 
  delay(1000);
}

void loop(){
    if (reading == 99){
        startTime = millis();
        while (((millis() - startTime) <= 1300) &&
                !digitalRead(14) &&
                digitalRead(15)){
                  motor1.drive(255);
                  motor2.drive(150);
                  delay(50);
                  motor1.brake();
                  motor2.brake();
        }    
    }
    if (reading == 108){
      motor1.drive(-255);
      motor2.drive(150);
      delay(1300);
      motor1.brake();
      motor2.brake();  
    }
    if (reading == 114){
      motor1.drive(255);
      motor2.drive(-150);
      delay(1300);
      motor1.brake();
      motor2.brake();  
    }
    reading = 0;
}

void receiveCommands(){
    reading = Wire.read(); 
}
