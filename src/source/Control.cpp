/*
 * Control.cpp
 *
 *  Created on: 2016.1.6
 *  Author: liboyang
 */
#include "Control.h"
bool IfControlWork()  {
	return true;
}
int set_interface_attribs (int fd, int speed, int parity) {
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
void set_blocking (int fd, int should_block) {
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
void AdjustDirection(point CurrentPosition,  point NextPosition,  int &CurrentDirection)  {
	if  (CurrentPosition.x == NextPosition.x)  {
		if  (NextPosition.y > CurrentPosition.y)  {
			switch(CurrentDirection)  {
			case EAST:
				break;
			case WEST:
				Turn180();
				break;
			case SOUTH:
				TurnLeft();
				break;
			case NORTH:
				TurnRight();
				break;
			}
			CurrentDirection = EAST;
			cout << "Current Direction: EAST" << endl;
		}
		else  {
			switch(CurrentDirection)  {
			case EAST:
				Turn180();
				break;
			case WEST:
				break;
			case SOUTH:
				TurnRight();
				break;
			case NORTH:
				TurnLeft();
				break;
			}
			CurrentDirection = WEST;
			cout << "Current Direction: WEST" << endl;
		}
	}
	else if  (CurrentPosition.y == NextPosition.y)  {
		if  (NextPosition.x > CurrentPosition.x)  {
			switch(CurrentDirection)  {
			case EAST:
				TurnRight();
				break;
			case WEST:
				TurnLeft();
				break;
			case SOUTH:
				break;
			case NORTH:
				Turn180();
				break;
			}
			CurrentDirection = SOUTH;
			cout << "Current Direction: SOUTH" << endl;
		}
		else  {
			switch(CurrentDirection)  {
			case EAST:
				TurnLeft();
				break;
			case WEST:
				TurnRight();
				break;
			case SOUTH:
				Turn180();
				break;
			case NORTH:
				break;
			}
			CurrentDirection = NORTH;
			cout << "Current Direction: NORTH" << endl;
		}
	}
}
void Turn180()  {
	cout << "Turn 180" << endl;
}
void TurnLeft()  {
	char *portname = "/dev/ttymxc3";
	int fd = open (portname, O_RDWR | O_NOCTTY | O_SYNC);
	if (fd < 0) {
		printf("error %d opening %s: %s", errno, portname, strerror (errno));
		return;
	}
	set_interface_attribs (fd, B115200, 0);
	set_blocking (fd, 0);
	write(fd, "3", 1);
	cout << "Turn Left" << endl;
}
void TurnRight()  {
	char *portname = "/dev/ttymxc3";
	int fd = open (portname, O_RDWR | O_NOCTTY | O_SYNC);
	if (fd < 0) {
		printf("error %d opening %s: %s", errno, portname, strerror (errno));
		return;
	}
	set_interface_attribs (fd, B115200, 0);
	set_blocking (fd, 0);
	write(fd, "4", 1);
	cout << "Turn Right" << endl;
}
void GoStraight()  {
	char *portname = "/dev/ttymxc3";
	int fd = open (portname, O_RDWR | O_NOCTTY | O_SYNC);
	if (fd < 0) {
		printf("error %d opening %s: %s", errno, portname, strerror (errno));
		return;
	}
	set_interface_attribs (fd, B115200, 0);
	set_blocking (fd, 0);
	write(fd, "1", 1);
	cout << "Go Straight!" << endl;
}
void Stop()  {
	char *portname = "/dev/ttymxc3";
	int fd = open (portname, O_RDWR | O_NOCTTY | O_SYNC);
	if (fd < 0) {
		printf("error %d opening %s: %s", errno, portname, strerror (errno));
		return;
	}
	set_interface_attribs (fd, B115200, 0);
	set_blocking (fd, 0);
	write(fd, "2", 1);
	cout << "Stop Successful" << endl;
}
void Back()  {

}
