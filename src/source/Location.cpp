/**
 * @file Location.cpp
 * @brief The implementation of class Location
 */

#include "Location.h"

/**
 * @fn Location::Location()
 * @brief Default constructor of class Location
 * @details Open the port to NFC, call function initial()
 */
Location::Location() {
	portname = "/dev/ttymxc3";
	fd = open(portname, O_RDWR | O_NOCTTY | O_SYNC);
	work = false;
	initial();
}

/**
 * @fn Location::~Location()
 */
Location::~Location() {}

/**
 * @fn Location::setInterfaceAttribs(int fd, int speed, int parity)
 * @brief setter function for port to NFC
 * @param fd port open return value
 * @param speed communicate speed
 * @param parity if parity set 0
 * @return error code
 */
int Location::setInterfaceAttribs(int fd, int speed, int parity) {
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
 * @fn Location::setBlocking(int fd, int should_block)
 * @brief setter function for port to NFC
 * @param fd port open return value
 * @param should_block whether the post should be block
 * @return error code
 */
void Location::setBlocking(int fd, int should_block) {
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
 * @fn Location::initial()
 * @brief call by constructor
 */
void Location::initial() {
	if (fd < 0) {
		printf("error %d opening %s: %s", errno, portname, strerror (errno));
		return;
	}
	setInterfaceAttribs (fd, B115200, 0);
	setBlocking (fd, 0);
	work = true;
	insertMap();
}

/**
 * @fn Location::doLoop()
 * @brief check if reach a new place, do all the time
 */
void Location::doLoop() {
	char buf[256], id[5];
	memset(buf, '\0', sizeof(buf));

	int n, i, cnt;
	while (true) {
		cnt = 0;
		while (cnt < 5) {
			n = read(fd, buf, sizeof(buf));
			if (n > 0) {
				 for (i = 0; i < n; i++) {
				 	id[cnt++] = buf[i];
				 }
			}
			sleep(0.1);
		}
		searchMap(buf2str(&id[0]));
		sleep(1);
	}
}

/**
 * @fn Location::buf2str(char* c)
 * @brief convert char[] to string in dex
 * @param pointer of char array
 * @return convert result
 */
string Location::buf2str(char* c) {
	char buf[50];
	sprintf(buf, "%x %x %x %x %x", c[0], c[1], c[2], c[3], c[4]);
	return string(buf);
}

/**
 * @fn Location::search(const string& s)
 * @brief search where the car is and update the current position
 * @param string of nfc id
 */
void Location::searchMap(const string& s) {
	map<string, point>::iterator it;
	it = m_map.find(s);
	if (it != m_map.end()) {
		Util::getCar().getMap().setCurrentPosition(it->second);
	} else {
		printf("Can not find current position, id =  %s\n", s.c_str());
	}
}

/**
 * @fn Location::insertMap()
 * @brief clear and construct the m_map
 */
void Location::insertMap() {
	m_map.clear();
	// Sample, TO DO: insert more
	m_map.insert(pair<string, point>("21 21 21 21 21", point(7, 4)));
}
