#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#define print1Byte(args) Serial1.write(args)
#define print1lnByte(args)  Serial1.write(args),Serial1.println()
#else
#include "WProgram.h"
#define print1Byte(args) Serial1.print(args,BYTE)
#define print1lnByte(args)  Serial1.println(args,BYTE)
#endif

// Set Directions of motor
int MoterRightDir1 = 13;
int MoterRightDir2 = 12;
int MoterLeftDir1 = 11;
int MoterLeftDir2 = 10;
// Save Motor Status
int motorStatus = 0;
int period = 100;
int Speed = 3; //Set speed of motor value of 0 to 255 analog
int Brake = 9; // Brake for motor usually unused so I always let sit 0

// Command to wake up NFC module
const unsigned char wake[24] = {
    0x55, 0x55, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x03, 0xfd, 0xd4, \
    0x14, 0x01, 0x17, 0x00};
// Command to get firmware version
const unsigned char firmware[9] = {
    0x00, 0x00, 0xFF, 0x02, 0xFE, 0xD4, 0x02, 0x2A, 0x00 };
// Command to read the tag
const unsigned char tag[11] = {
    0x00, 0x00, 0xFF, 0x04, 0xFC, 0xD4, 0x4A, 0x01, 0x00, 0xE1, 0x00 };
// Standard ACK Template
const unsigned char std_ACK[25] = {
    0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x0C, \
    0xF4, 0xD5, 0x4B, 0x01, 0x01, 0x00, 0x04, 0x08, 0x04, 0x00, \
    0x00, 0x00, 0x00, 0x4b, 0x00};

unsigned char old_id[5]; // The last receive id
unsigned char receive_ACK[25]; // Command receiving buffer

/******** Custommed Function ********/

void goStop(){
    digitalWrite(MoterRightDir1, LOW);
    digitalWrite(MoterRightDir2, LOW);
    digitalWrite(MoterLeftDir1, LOW);
    digitalWrite(MoterLeftDir2, LOW);
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
}

void goStraight(){
	digitalWrite(MoterRightDir1, HIGH);
	digitalWrite(MoterRightDir2, HIGH);
	digitalWrite(MoterLeftDir1, HIGH);
	digitalWrite(MoterLeftDir2, HIGH);
}

void goLeft(){
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

void copy_id(void) { //save old id
    int ai, oi;
    for (oi = 0, ai = 19; oi < 5; oi++, ai++) {
        old_id[oi] = receive_ACK[ai];
    }
} 

char cmp_id(void) { //return true if find id is old
    int ai, oi;
    for (oi = 0, ai = 19; oi < 5; oi++, ai++) {
        if (old_id[oi] != receive_ACK[ai])
            return 0;
    }
    return 1;
}
 
int test_ACK(void) { // return true if receive_ACK accord with std_ACK
    int i;
    for (i = 0; i < 19; i++) {
        if (receive_ACK[i] != std_ACK[i])
            return 0;
    }
    return 1;
}
 
void send_id(void) { //send id to PC
    int i;
    Serial.print ("ID: ");
    for (i = 19; i <= 23; i++) {
        Serial.print(receive_ACK[i], HEX);
        Serial.print(" ");
    }
    Serial.println();
}

void send_id2(void) { //send id to PC
    int i;
    for (i = 19; i <= 23; i++) {
        Serial.write(receive_ACK[i]);
    }
}

void UART1_Send_Byte(unsigned char command_data) { //send byte to device
    print1Byte(command_data);
#if defined(ARDUINO) && ARDUINO >= 100
    Serial1.flush(); // complete the transmission of outgoing serial data 
#endif
} 

void UART_Send_Byte(unsigned char command_data) { //send byte to PC
    Serial.print(command_data,HEX);
    Serial.print(" ");
}

void read_ACK(unsigned char temp) { //read ACK into reveive_ACK[]
    unsigned char i;
    for(i = 0; i < temp; i++) {
        receive_ACK[i] = Serial1.read();
    }
}
 
void wake_card(void) {//send wake[] to device
    unsigned char i;
    for(i = 0; i < 24; i++) //send command
        UART1_Send_Byte(wake[i]);
}

void send_tag(void) { //send tag[] to device
    unsigned char i;
    for(i = 0; i < 11; i++) //send command
        UART1_Send_Byte(tag[i]);
}

/******** Arduino Function ********/

void serialEvent() {
    while(Serial.available()) {
        char a = (char)Serial.read();
        if (a == '0') {
            motorStatus = 0;
            goStraight(); 
        }
        else if (a == '1') {
            motorStatus = 1;
            goStraight50(); 
        }
        else if (a == '2') {
            motorStatus = 2;
            goStop(); 
        }
        else if (a == '3') {
            motorStatus = 3;
            goLeft(); 
        }
        else if (a == '4') {
            motorStatus = 4;
            goRight(); 
        }
        else if (a == '5') {
            motorStatus = 5;
            goStraight30(); 
        }
    }
}

void setup() {
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
    wake_card();
    delay(100);
    read_ACK(15);
    delay(100);
}

void loop() {
    send_tag(); 
    read_ACK(25);
    delay(100);
    if (!cmp_id()) {
        if (test_ACK()) {
            send_id2();
            delay(100);
        }
    }
    copy_id();
    delay(100);
}