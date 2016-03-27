/**
 * @file Motor.cpp
 * @brief The implementation of class Motor
 */

#include "Motor.h"

/**
 * @fn Motor::Motor()
 * @brief Default constructor of class Motor
 * @details Call function initial()
 */
Motor::Motor() {
	work = false;
	initial();
}

/**
 * @fn Motor::~Motor()
 */
Motor::~Motor() {}

/**
 * @fn Motor::initial()
 * @brief call by constructor
 */
void Motor::initial() {
	if (callSystem("echo out > /sys/class/gpio/"MoterRightDir1"/direction") 
		&& callSystem("echo out > /sys/class/gpio/"MoterRightDir2"/direction")
		&& callSystem("echo out > /sys/class/gpio/"MoterLeftDir1"/direction")
		&& callSystem("echo out > /sys/class/gpio/"MoterLeftDir2"/direction")) {
		work = true;
		stop();
	}
}

/**
 * @fn Motor::turn180()
 * @brief Control function for Motor
 */
void Motor::turn180() {
	// TODO: to be implemented
	printf("Turn 180\n");
}

/**
 * @fn Motor::turnLeft()
 * @brief Control function for Motor
 */
void Motor::turnLeft() {
	if(isWork()) {
		callSystem("echo 0 > /sys/class/gpio/"MoterRightDir1"/value");
		callSystem("echo 1 > /sys/class/gpio/"MoterRightDir2"/value");
		callSystem("echo 1 > /sys/class/gpio/"MoterLeftDir1"/value");
		callSystem("echo 0 > /sys/class/gpio/"MoterLeftDir2"/value");
		printf("Turn Left!\n");
	} else {
		printf("Motor is not working...\n");
	}
}

/**
 * @fn Motor::turnRight()
 * @brief Control function for Motor
 */
void Motor::turnRight() {
	if (isWork()) {	
		callSystem("echo 1 > /sys/class/gpio/"MoterRightDir1"/value");
		callSystem("echo 0 > /sys/class/gpio/"MoterRightDir2"/value");
		callSystem("echo 0 > /sys/class/gpio/"MoterLeftDir1"/value");
		callSystem("echo 1 > /sys/class/gpio/"MoterLeftDir2"/value");
		printf("Turn Right!\n");
	} else {
		printf("Motor is not working...\n");
	}
}

/**
 * @fn Motor::goStraight()
 * @brief Control function for Motor
 */
void Motor::goStraight() {
	if (isWork()) {
		callSystem("echo 1 > /sys/class/gpio/"MoterRightDir1"/value");
		callSystem("echo 1 > /sys/class/gpio/"MoterRightDir2"/value");
		callSystem("echo 1 > /sys/class/gpio/"MoterLeftDir1"/value");
		callSystem("echo 1 > /sys/class/gpio/"MoterLeftDir2"/value");
		printf("Go Straight!\n");
	} else {
		printf("Motor is not working...\n");
	}
}

/**
 * @fn Motor::stop()
 * @brief Control function for Motor
 */
void Motor::stop() {
	if (isWork()) {
		callSystem("echo 0 > /sys/class/gpio/"MoterRightDir1"/value");
		callSystem("echo 0 > /sys/class/gpio/"MoterRightDir2"/value");
		callSystem("echo 0 > /sys/class/gpio/"MoterLeftDir1"/value");
		callSystem("echo 0 > /sys/class/gpio/"MoterLeftDir2"/value");
		printf("Stop Successful\n");
	} else {
		printf("Motor is not working...\n");
	}
}

/**
 * @fn Motor::isWork()
 * @brief Check if the motor can work
 * @return true  - can work
 * @return false - otherwise
 */
bool Motor::isWork() {
	return work;
}

/**
 * @fn Motor::callSystem()
 * @brief Exec system command and return result
 * @return true  - successfully
 * @return false - otherwise
 */
bool Motor::callSystem(const string& s) {
	int status = system(s.c_str());
	if (status == -1) {
		printf("System error\n");
	} else {
		int exitStatus = WIFEXITED(status);
		if (exitStatus) {
			int exitCode = WEXITSTATUS(status);
			if (exitCode == 0) {
				return true;
			} else {
				printf("run command fail and exit code is %d\n", exitCode);
			}
		} else {
			printf("exit status = %d\n", exitStatus);
		}
	}
	return false;
}