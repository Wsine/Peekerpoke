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
#include <stdlib.h>

#define MAP_ROWS 10
#define MAP_COLUMNS 10
#define PATH_LENGTH 50

/*
 * define go_east go_west go_south go_north
 */
const int oper_x[4] = {0, 0, 1, -1};
const int oper_y[4] = {-1, 1, 0, 0};

enum direction {
	d_east = 1,
	d_west = 2,
	d_south = 3,
	d_north = 4
};

struct point {
	int x, y, step;
	point() {}
	point(int _x, int _y, int _step = 0) {
		x = _x;
		y = _y;
		step = _step;
	}
	bool operator== (const point& other) {
		return (x == other.x && y == other.y);
	}
};

class Map {
private:
	int matrix[MAP_ROWS][MAP_COLUMNS];
	bool visited[MAP_ROWS][MAP_COLUMNS];
	point path[PATH_LENGTH];
	point destination;
	point currentPosition;
	direction currentDirection;
	point nextPosition;
/* Method */
public:
	Map();
	~Map();
	void updateNextPosition();
	bool nextPositionAvilable();
	direction getCurrentDirection();
	void setCurrentDirection(direction _currentDirection);
	int compareCurrent2nextDirection(char axis);
	void printPosition(int index);
	bool arriveDestination();
	void printMap();
	point getNextPosition();
	int getMapAtPosition(const int& position);
private:
	void initMatrix();
	void initDestination();
	void initCurrentPosition();
	void initCurrentDirection();
	void initial();
	void initDfs();
	bool dfsPointValid(const point& p);
	void printPath(int length);
	bool dfs(point cur, int step);
};

#endif