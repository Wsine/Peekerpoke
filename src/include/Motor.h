#ifndef MOTOR_H_
#warning "Ready to define MOTOR_H_"
#define MOTOR_H_

#include <iostream>
#include <termios.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
using namespace std;

class Motor {
private:
	char *portname;
	int fd;
	bool work;
/* Method */
public:
	Motor();
	~Motor();
	bool isWork();
	void turn180();
	void turnLeft();
	void turnRight();
	void goStraight();
	void stop();
private:
	int setInterfaceAttribs(int fd, int speed, int parity);
	void setBlocking(int fd, int should_block);
	void initial();
};

#endif