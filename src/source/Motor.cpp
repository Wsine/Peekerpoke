/**
 * @file Motor.cpp
 * @brief The implementation of class Motor
 */

#include "Motor.h"

/**
 * @fn Motor::Motor()
 * @brief Default constructor of class Motor
 * @details Open the port to NFC, call function initial()
 */
Motor::Motor() {
	portname = "/dev/ttymxc3";
	fd = open(portname, O_RDWR | O_NOCTTY | O_SYNC);
	work = false;
	initial();
}

/**
 * @fn Motor::~Motor()
 */
Motor::~Motor() {}

/**
 * @fn Motor::setInterfaceAttribs(int fd, int speed, int parity)
 * @brief setter function for port to NFC
 * @param fd port open return value
 * @param speed communicate speed
 * @param parity if parity set 0
 * @return error code
 */
int Motor::setInterfaceAttribs(int fd, int speed, int parity) {
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

/**
 * @fn Motor::setBlocking(int fd, int should_block)
 * @brief setter function for port to NFC
 * @param fd port open return value
 * @param should_block whether the post should be block
 * @return error code
 */
void Motor::setBlocking(int fd, int should_block) {
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

/**
 * @fn Motor::initial()
 * @brief call by constructor
 */
void Motor::initial() {
	if (fd < 0) {
		printf("error %d opening %s: %s", errno, portname, strerror (errno));
		return;
	}
	setInterfaceAttribs (fd, B115200, 0);
	setBlocking (fd, 0);
	work = true;
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
		write(fd, "3", 1);
		printf("Turn Left\n");		
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
		write(fd, "4", 1);
		printf("Turn Right\n");
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
		write(fd, "1", 1);
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
		write(fd, "2", 1);
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