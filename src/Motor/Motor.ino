// All of these set the pin locations
const unsigned char wake[24]={0x55,0x55,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0x03,0xfd,0xd4,0x14,0x01,0x17,0x00};//wake up NFC module
const unsigned char firmware[9]={0x00,0x00,0xFF,0x02,0xFE,0xD4,0x02,0x2A,0x00};//
const unsigned char tag[11]={0x00,0x00,0xFF,0x04,0xFC,0xD4,0x4A,0x01,0x00,0xE1,0x00};//detecting tag command

unsigned char receive_ACK[25];//Command receiving buffer
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
#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#define print1Byte(args) Serial1.write(args)
#define print1lnByte(args)  Serial1.write(args),Serial1.println()
#else
#include "WProgram.h"
#define print1Byte(args) Serial1.print(args,BYTE)
#define print1lnByte(args)  Serial1.println(args,BYTE)
#endif
void setup(){
	// This defines the speed of arduino serial
	Serial.begin(115200);
        Serial1.begin(115200);
        wake_card();
        delay(100);
        array_ACK(15);
        delay(100);
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
	analogWrite(Speed, 0);
}
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
        //goStop();  
        //delay(period * 0.1);
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
void loop(){
        //while(Serial.available() > 0)  {
        int a = Serial.read();
        //}
	if(a == '0'){ // no thing to do with this
		//digitalWrite(MoterRightDir1, LOW);
		//analogWrite(Speed, 0);
		//digitalWrite(MoterRightDir2, HIGH);
		//analogWrite(MoterLeftDir1, 255);
                goStraight();
	}
	else if(a == '1'){
		goStraight50();
	}
	else if(a == '2'){
		goStop();
	}
	else if(a == '3'){
		goLeft();
	}
	else if(a == '4'){
		goRight();
	}
        else if(a == '5'){
                goStraight30();
        }
        read_tag();
        delay(100);
        array_ACK(25);
        delay(100);
        dsplay(25);
}
void serialEvent()  {
  while(Serial.available())  {
    char a = (char)Serial.read();
    if  (a == '0')  {
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
void UART1_Send_Byte(unsigned char command_data)
{
  //Serial1.print(command_data);
  print1Byte(command_data);// command send to device
  #if defined(ARDUINO) && ARDUINO >= 100
    Serial1.flush();// complete the transmission of outgoing serial data 
    #endif
} 

void UART_Send_Byte(unsigned char command_data)//, unsigned charBYTE
{
  Serial.write(command_data);
  //Serial.print(command_data,HEX);
  //Serial.print(" ");
} 


void array_ACK(unsigned char temp)
{
    for(unsigned char i=0;i<temp;i++)                                           
    {
       receive_ACK[i]= Serial1.read();
       delay(100);
    }
}

void wake_card(void)
{
    unsigned char i;
    for(i=0;i<24;i++) //send command
   UART1_Send_Byte(wake[i]);
}

void firmware_version(void)
{
    unsigned char i;
    for(i=0;i<9;i++) //send command
    UART1_Send_Byte(firmware[i]);
}

void read_tag(void)
{
    unsigned char i;
    for(i=0;i<11;i++) //send command
    UART1_Send_Byte(tag[i]);
}

void dsplay(unsigned char tem)
{
    unsigned char i;
    for(i=0;i<tem;i++) //send command
    UART_Send_Byte(receive_ACK[i]);
}
