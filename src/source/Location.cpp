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
	m_map.insert(pair<string, point>("3c 71 67 32 88", point(0, 0)));
	m_map.insert(pair<string, point>("6c 45 77 32 74", point(0, 1)));
	m_map.insert(pair<string, point>("b2 10 59 f3 c0", point(0, 2)));
	m_map.insert(pair<string, point>("82 48 56 f3 bb", point(0, 3)));
	m_map.insert(pair<string, point>("32 71 4a f3 ee", point(0, 4)));
	m_map.insert(pair<string, point>("9c cc 62 32 d2", point(0, 5)));
	m_map.insert(pair<string, point>("2c db 5c 32 39", point(0, 6)));
	m_map.insert(pair<string, point>("4c 9a 5f 32 57", point(0, 7)));
	m_map.insert(pair<string, point>("6c 31 76 32 89", point(0, 8)));
	m_map.insert(pair<string, point>("dc 3d 66 32 1d", point(0, 9)));
	m_map.insert(pair<string, point>("cc a5 6c 32 bf", point(1, 0)));
	m_map.insert(pair<string, point>("9c 3d 66 32 5d", point(1, 1)));
	m_map.insert(pair<string, point>("92 50 e9 f2 11", point(1, 2)));
	m_map.insert(pair<string, point>("b2 95 49 f3 4b", point(1, 3)));
	m_map.insert(pair<string, point>("fc 47 75 32 e4", point(1, 4)));
	m_map.insert(pair<string, point>("dc 26 51 2b 50", point(1, 5)));
	m_map.insert(pair<string, point>("12 17 2e f3 84", point(1, 6)));
	m_map.insert(pair<string, point>("3c 3c 70 32 b4", point(1, 7)));
	m_map.insert(pair<string, point>("52 91 3b f3 bd", point(1, 8)));
	m_map.insert(pair<string, point>("52 1b 43 f3 2b", point(1, 9)));
	m_map.insert(pair<string, point>("d2 f5 36 f3 de", point(2, 0)));
	m_map.insert(pair<string, point>("b2 9b 49 f3 45", point(2, 1)));
	m_map.insert(pair<string, point>("22 29 37 f3 59", point(2, 2)));
	m_map.insert(pair<string, point>("72 ba 6f f3 40", point(2, 3)));
	m_map.insert(pair<string, point>("ac 69 74 32 13", point(2, 4)));
	m_map.insert(pair<string, point>("8c 7a 74 32 22", point(2, 5)));
	m_map.insert(pair<string, point>("4c 11 54 2b f2", point(2, 6)));
	m_map.insert(pair<string, point>("7c dc 5d 32 e7", point(2, 7)));
	m_map.insert(pair<string, point>("2c 73 7b 32 82", point(2, 8)));
	m_map.insert(pair<string, point>("72 40 37 f3 f2", point(2, 9)));
	m_map.insert(pair<string, point>("bc f9 62 32 85", point(3, 0)));
	m_map.insert(pair<string, point>("c 11 66 32 19",  point(3, 1)));
	m_map.insert(pair<string, point>("9c 1a 64 32 82", point(3, 2)));
	m_map.insert(pair<string, point>("cc 31 70 32 2f", point(3, 3)));
	m_map.insert(pair<string, point>("7c ba 79 32 ed", point(3, 4)));
	m_map.insert(pair<string, point>("4c 7b 74 32 61", point(3, 5)));
	m_map.insert(pair<string, point>("3c 98 69 32 5f", point(3, 6)));
	m_map.insert(pair<string, point>("dc 93 5f 32 ce", point(3, 7)));
	m_map.insert(pair<string, point>("dc 58 66 32 2",  point(3, 8)));
	m_map.insert(pair<string, point>("6c 9 70 32 b7",  point(3, 9)));
	m_map.insert(pair<string, point>("7c 37 76 32 73", point(4, 0)));
	m_map.insert(pair<string, point>("6c b 66 32 bf",  point(4, 1)));
	m_map.insert(pair<string, point>("42 9d 49 f3 b3", point(4, 2)));
	m_map.insert(pair<string, point>("7c db 5a 2b f2", point(4, 3)));
	m_map.insert(pair<string, point>("cc 8c 74 32 d0", point(4, 4)));
	m_map.insert(pair<string, point>("d2 b1 4a f3 e",  point(4, 5)));
	m_map.insert(pair<string, point>("92 f1 0 f3 58",  point(4, 6)));
	m_map.insert(pair<string, point>("b2 2d 46 f3 b6", point(4, 7)));
	m_map.insert(pair<string, point>("7c 6a 59 32 5d", point(4, 8)));
	m_map.insert(pair<string, point>("fc 1 7a 32 25",  point(4, 9)));
	m_map.insert(pair<string, point>("e2 a1 49 f3 f",  point(5, 0)));
	m_map.insert(pair<string, point>("5c 47 66 32 93", point(5, 1)));
	m_map.insert(pair<string, point>("5c 51 73 32 7c", point(5, 2)));
	m_map.insert(pair<string, point>("5c 14 70 32 bc", point(5, 3)));
	m_map.insert(pair<string, point>("dc 74 57 32 f5", point(5, 4)));
	m_map.insert(pair<string, point>("1c b2 69 32 65", point(5, 5)));
	m_map.insert(pair<string, point>("cc 39 70 32 27", point(5, 6)));
	m_map.insert(pair<string, point>("7c 35 70 32 7b", point(5, 7)));
	m_map.insert(pair<string, point>("42 53 ee c5 86", point(5, 8)));
	m_map.insert(pair<string, point>("9c 77 64 2b 2c", point(5, 9)));
	m_map.insert(pair<string, point>("9c 90 54 2b 23", point(6, 0)));
	m_map.insert(pair<string, point>("2 be 34 f3 e7",  point(6, 1)));
	m_map.insert(pair<string, point>("6c bb 62 32 13", point(6, 2)));
	m_map.insert(pair<string, point>("7c a3 69 32 14", point(6, 3)));
	m_map.insert(pair<string, point>("cc 47 72 32 17", point(6, 4)));
	m_map.insert(pair<string, point>("1c 3e 70 32 d2", point(6, 5)));
	m_map.insert(pair<string, point>("5c 49 69 32 8e", point(6, 6)));
	m_map.insert(pair<string, point>("8c 17 70 32 89", point(6, 7)));
	m_map.insert(pair<string, point>("ac f3 65 32 98", point(6, 8)));
	m_map.insert(pair<string, point>("b2 8a 5c f3 43", point(6, 9)));
	m_map.insert(pair<string, point>("c2 e5 51 f3 e3", point(7, 0)));
	m_map.insert(pair<string, point>("82 df 63 f3 17", point(7, 1)));
	m_map.insert(pair<string, point>("e2 c1 67 f3 d1", point(7, 2)));
	m_map.insert(pair<string, point>("2 ae 4c f3 df",  point(7, 3)));
	m_map.insert(pair<string, point>("7c 3f 75 32 6c", point(7, 4)));
	m_map.insert(pair<string, point>("92 fa 58 f3 f7", point(7, 5)));
	m_map.insert(pair<string, point>("9c cc 5f 32 d5", point(7, 6)));
	m_map.insert(pair<string, point>("2 67 49 f3 29",  point(7, 7)));
	m_map.insert(pair<string, point>("32 a3 48 f3 be", point(7, 8)));
	m_map.insert(pair<string, point>("d2 83 6a f3 1c", point(7, 9)));
	m_map.insert(pair<string, point>("bc 69 73 32 4",  point(8, 0)));
	m_map.insert(pair<string, point>("2c 50 73 32 ad", point(8, 1)));
	m_map.insert(pair<string, point>("cc 8f 5f 32 e2", point(8, 2)));
	m_map.insert(pair<string, point>("a2 58 ad f2 35", point(8, 3)));
	m_map.insert(pair<string, point>("dc 94 79 32 b3", point(8, 4)));
	m_map.insert(pair<string, point>("f2 ad 6c f3 d0", point(8, 5)));
	m_map.insert(pair<string, point>("72 97 5e f3 74", point(8, 6)));
	m_map.insert(pair<string, point>("2c c1 77 32 38", point(8, 7)));
	m_map.insert(pair<string, point>("8c 47 71 32 58", point(8, 8)));
	m_map.insert(pair<string, point>("9c 3f 68 32 59", point(8, 9)));
	m_map.insert(pair<string, point>("dc 58 71 32 f7", point(9, 0)));
	m_map.insert(pair<string, point>("d2 d8 3b f3 f6", point(9, 1)));
	m_map.insert(pair<string, point>("72 de 4a f3 41", point(9, 2)));
	m_map.insert(pair<string, point>("9c f0 64 32 ac", point(9, 3)));
	m_map.insert(pair<string, point>("7c 35 77 32 74", point(9, 4)));
	m_map.insert(pair<string, point>("c 82 57 32 b7",  point(9, 5)));
	m_map.insert(pair<string, point>("2c ac 6a 32 5a", point(9, 6)));
	m_map.insert(pair<string, point>("ec c8 6a 32 7e", point(9, 7)));
	m_map.insert(pair<string, point>("4c 2c 77 32 ad", point(9, 8)));
	m_map.insert(pair<string, point>("7c 67 67 32 52", point(9, 9)));

