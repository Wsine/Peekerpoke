#include "../include/Motor.h"

Motor::Motor() {
	portname = "/dev/ttymxc3";
	fd = open(portname, O_RDWR | O_NOCTTY | O_SYNC);
	work = false;
	initial();
}

Motor::~Motor() {}

int Motor::set_interface_attribs(int fd, int speed, int parity) {
	struct termios tty;
	memset (&tty, 0, sizeof tty);
	if (tcgetattr (fd, &tty) != 0){
		printf ("error from tcgetattr", errno);
		return -1;
	}
	cfsetospeed (&tty, speed);
	cfsetispeed (&tty, speed);
	tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8;
	tty.c_iflag &= ~IGNBRK;
	tty.c_lflag = 0;
	tty.c_oflag = 0;
	tty.c_cc[VMIN]  = 0;
	tty.c_cc[VTIME] = 5;
	tty.c_iflag &= ~(IXON | IXOFF | IXANY);
	tty.c_cflag |= (CLOCAL | CREAD);
	tty.c_cflag &= ~(PARENB | PARODD);
	tty.c_cflag |= parity;
	tty.c_cflag &= ~CSTOPB;
	tty.c_cflag &= ~CRTSCTS;
	if (tcsetattr (fd, TCSANOW, &tty) != 0){
		printf ("error %d from tcsetattr", errno);
		return -1;
	}
	return 0;
}

void Motor::set_blocking(int fd, int should_block) {
	struct termios tty;
	memset (&tty, 0, sizeof tty);
	if (tcgetattr (fd, &tty) != 0){
		printf ("error %d from tggetattr", errno);
		return;
	}
	tty.c_cc[VMIN]  = should_block ? 1 : 0;
	tty.c_cc[VTIME] = 5;
	if (tcsetattr (fd, TCSANOW, &tty) != 0)
		printf ("error %d setting term attributes", errno);
}

void Motor::initial() {
	if (fd < 0) {
		printf("error %d opening %s: %s", errno, portname, strerror (errno));
		return;
	}
	set_interface_attribs (fd, B115200, 0);
	set_blocking (fd, 0);
	work = true;
}

void Motor::turn_180() {
	// TODO: to be implemented
	printf("Turn 180\n");
}

void Motor::turn_left() {
	if(is_work()) {
		write(fd, "3", 1);
		printf("Turn Left\n");		
	} else {
		printf("Motor is not working...\n");
	}
}

void Motor::turn_right() {
	if (is_work()) {	
		write(fd, "4", 1);
		printf("Turn Right\n");
	} else {
		printf("Motor is not working...\n");
	}
}

void Motor::go_straight() {
	if (is_work()) {
		write(fd, "1", 1);
		printf("Go Straight!\n");
	} else {
		printf("Motor is not working...\n");
	}
}

void Motor::stop() {
	if (is_work()) {
		write(fd, "2", 1);
		printf("Stop Successful\n");
	} else {
		printf("Motor is not working...\n");
	}
}

bool Motor::is_work() {
	return work;
}