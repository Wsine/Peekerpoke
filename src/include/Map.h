#ifndef MAP_H_
#define MAP_H_

#include <iostream>
#include <termios.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>

#define MAP_ROWS 10
#define MAP_COLUMNS 10

enum direction {
	d_east = 1,
	d_west = 2,
	d_south = 3,
	d_north = 4
};

struct point {
	int x, y, step;
	point(int _x, int _y, int _step) {
		x = _x;
		y = _y;
		step = _step;
	}
};

class Map {
private:
	int matrix[MAP_ROWS][MAP_COLUMNS];
	point destination;
	point currentPosition;
/* Method */
public:
	Map();
	~Map();
private:
	void set_matrix();
	void set_destination();
	void initial();
	void print();
	point get_next_position();
};

#endif