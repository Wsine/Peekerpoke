#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#define print1Byte(args) Serial1.write(args)
#define print1lnByte(args)  Serial1.write(args),Serial1.println()
#else
#include "WProgram.h"
#define print1Byte(args) Serial1.print(args,BYTE)
#define print1lnByte(args)  Serial1.println(args,BYTE)
#endif

//Set Directions of motor
int MoterRightDir1 = 13;
int MoterRightDir2 = 12;
int MoterLeftDir1 = 11;
int MoterLeftDir2 = 10;
//Save Motor Status
int motorStatus = 0;
int period = 100;
int Speed = 3; //Set speed of motor value of 0 to 255 analog
int Brake = 9; // Brake for motor usually unused so I always let sit 0


void goStraight30()  {
    digitalWrite(MoterRightDir1, HIGH);
    digitalWrite(MoterRightDir2, HIGH);
    digitalWrite(MoterLeftDir1, HIGH);
    digitalWrite(MoterLeftDir2, HIGH);
    delay(period * 0.3);
    goStop();  
    delay(period * 0.7);
}

void goStraight50()  {
    digitalWrite(MoterRightDir1, HIGH);
	digitalWrite(MoterRightDir2, HIGH);
	digitalWrite(MoterLeftDir1, HIGH);
	digitalWrite(MoterLeftDir2, HIGH);
    delay(period * 0.5);

}

void goStraight(){
	digitalWrite(MoterRightDir1, HIGH);
	digitalWrite(MoterRightDir2, HIGH);
	digitalWrite(MoterLeftDir1, HIGH);
	digitalWrite(MoterLeftDir2, HIGH);
}

void goStop(){
	digitalWrite(MoterRightDir1, LOW);
	digitalWrite(MoterRightDir2, LOW);
	digitalWrite(MoterLeftDir1, LOW);
	digitalWrite(MoterLeftDir2, LOW);
}

void goLeft(){
	//digitalWrite(MoterRightDir1, LOW);
        digitalWrite(MoterRightDir1, HIGH);
	digitalWrite(MoterRightDir2, HIGH);
	digitalWrite(MoterLeftDir1, HIGH);
	digitalWrite(MoterLeftDir2, LOW);
}

void goRight(){
	digitalWrite(MoterRightDir1, HIGH);
	digitalWrite(MoterRightDir2, LOW);
	digitalWrite(MoterLeftDir1, LOW);
	digitalWrite(MoterLeftDir2, HIGH);
}

void serialEvent()  {
    while(Serial.available())  {
        char a = (char)Serial.read();
        if (a == '0')  {
            motorStatus = 0;
            goStraight(); 
        }
        else if  (a == '1')  {
            motorStatus = 1;
            goStraight50(); 
        }
        else if  (a == '2')  {
            motorStatus = 2;
            goStop(); 
        }
        else if  (a == '3')  {
            motorStatus = 3;
            goLeft(); 
        }
        else if  (a == '4')  {
            motorStatus = 4;
            goRight(); 
        }
        else if  (a == '5')  {
            motorStatus = 5;
            goStraight30(); 
        }
    }
}

void setup(){
    // This defines the speed of arduino serial
    Serial.begin(115200);
    Serial1.begin(115200);
    // These set modes of pins
    pinMode(MoterRightDir1, OUTPUT);
    pinMode(MoterRightDir2, OUTPUT);
    pinMode(MoterLeftDir1, OUTPUT);
    pinMode(MoterLeftDir2, OUTPUT);
    pinMode(Speed, OUTPUT);
    pinMode(Brake, OUTPUT);
    // Ininitalizes pins. Not moving
    digitalWrite(MoterRightDir1, LOW);
    digitalWrite(MoterRightDir2, LOW);
    digitalWrite(MoterLeftDir1, LOW);
    digitalWrite(MoterLeftDir2, LOW);
    digitalWrite(Brake, LOW);
    // analogWrite(Speed, 0);
}

void loop(){
    
}