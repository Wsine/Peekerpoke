/**
 * @file Map.h
 * @brief The declaration of class Map
 */

#ifndef MAP_H_
#define MAP_H_

#include <iostream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

/**
 * @def MAP_ROWS
 * @brief the rows of the map
 */
#define MAP_ROWS 10
/**
 * @def MAP_COLUMNS
 * @brief the columns of the map
 */
#define MAP_COLUMNS 10
/**
 * @def PATH_LENGTH
 * @brief the max length of the road to nex position
 */
#define PATH_LENGTH 50
/**
 * @def INF_POINT
 * @brief the point which means can not reach
 */
#define INF_POINT point(-1, -1)

/**
 * @brief define go_east go_west go_south go_north
 */
const int oper_x[4] = {0, 0, 1, -1};
/**
 * @brief define go_east go_west go_south go_north
 */
const int oper_y[4] = {-1, 1, 0, 0};

/**
 * @brief enum of four direction
 */
enum direction {
	d_east = 1,
	d_west = 2,
	d_south = 3,
	d_north = 4
};

struct point {
	int x, y, step;
	point();
	point(int _x, int _y, int _step = 0);
	bool operator== (const point& other);
	bool operator!= (const point& other);
	string toString();
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
	point getCurrentPosition();
	int getMapAtPosition(const int& position);
	void setCurrentPosition(const point& p);
	void setMapAtPosition(const point& p, const int& value);
private:
	void initMatrix();
	void initDestination();
	void initCurrentAndNextPosition();
	void initCurrentDirection();
	void initial();
	void initDfs();
	bool dfsPointValid(const point& p);
	void printPath(int length);
	bool dfs(point cur, int step);
};

#endif