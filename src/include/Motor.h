#ifndef MOTOR_H_
#define MOTOR_H_

#include <iostream>
#include <termios.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

class Motor {
private:
	char *portname;
	int fd;
	bool work;
/* Method */
public:
	Motor();
	~Motor();
private:
	int set_interface_attribs(int fd, int speed, int parity);
	void set_blocking(int fd, int should_block);
	void initial();
	void turn_180();
	void turn_left();
	void turn_right();
	void go_straight();
	void stop();
	bool is_work();
};

#endif