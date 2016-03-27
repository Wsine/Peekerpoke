#ifndef LOCATION_H_
#define LOCATION_H_

/**
 * @file Location.h
 * @brief The declaration of class Location
 */

#include <iostream>
#include <termios.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <map>
#include "Map.h"
#include "Util.h"
using namespace std;

class Location {
private:
	char *portname;
	int fd;
	bool work;
	map<string, point> m_map;
/* Method */
public:
	Location();
	~Location();
	bool isWork();
private:
	int setInterfaceAttribs(int fd, int speed, int parity);
	void setBlocking(int fd, int should_block);
	void initial();
	void doLoop();
	string buf2str(char* c);
	void searchMap(const string& s);
	void insertMap();
};

#endif
