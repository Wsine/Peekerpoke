#ifndef MOTOR_H_
#define MOTOR_H_

/**
 * @file Motor.h
 * @brief The declaration of class Motor
 */

#include <iostream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
using namespace std;

#define MoterRightDir1 "gpio123"
#define MoterRightDir2 "gpio125"
#define MoterLeftDir1 "gpio127"
#define MoterLeftDir2 "gpio134"

class Motor {
private:
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
	void initial();
	bool callSystem(const string& s);
};

#endif