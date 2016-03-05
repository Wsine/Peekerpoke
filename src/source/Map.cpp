/**
 * @file Map.cpp
 * @brief The implementation of class Map
 */

#include "Map.h"
#include "Util.h"

/**
 * @fn Map::Map()
 * @brief call the function initial()
 */
Map::Map() {
	initial();
}

/**
 * @fn Map::~Map()
 */
Map::~Map() {}

/**
 * @fn Map::initMatrix()
 * @brief initial the map according to your idea
 */
void Map::initMatrix() {
	int i, j;
	/* set the whole map obstacle */
	for (i = 0; i < MAP_ROWS; i++) {
		for (j = 0; j < MAP_COLUMNS; j++) {
			matrix[i][j] = 1;
		}
	}

	/* set the avilable positions */
	for (i = 0; i < MAP_ROWS; i++) {
		matrix[i][4] = 0;
	}
	for (i = 0; i < MAP_COLUMNS; i++) {
		matrix[4][i] = 0;
	}
}

/**
 * @fn Map::initDestination()
 * @brief initial the distination according to your idea
 */
void Map::initDestination() {
	destination = point(9, 4);
}

/**
 * @fn Map::initCurrentAndNextPosition()
 * @brief initial the current position according to your idea
 * @brief initial the next position same as current position
 */
void Map::initCurrentAndNextPosition() {
	currentPosition = point(5, 4);
	nextPosition = currentPosition;
}

/**
 * @fn Map::initCurrentDirection()
 * @brief initial the current direction according to your idea
 */
void Map::initCurrentDirection() {
	currentDirection = d_east;
}

/**
 * @fn Map::initial()
 * @brief initial the Map parameters
 */
void Map::initial() {
	initMatrix();
	initDestination();
	initCurrentAndNextPosition();
	initCurrentDirection();
}

/**
 * @fn Map::printMap()
 * @brief Print the map in a specific format
 */
void Map::printMap() {
	printf("Current map:\n");
	for (int i = 0; i < MAP_ROWS; i++) {
		for (int j = 0; j < MAP_COLUMNS; j++) {
			printf("%d ", matrix[i][j]);
		}
		printf("\n");
	}
	printf("\n\n");
}

/**
 * @fn Map::initDfs()
 * @brief initial the dfs parameters for function dfs()
 */
void Map::initDfs() {
	memset(visited, false, sizeof(visited));
	visited[currentPosition.x][currentPosition.y] = true;
	currentPosition.step = 1;
}

/**
 * @fn Map::dfsPointValid(const point& p)
 * @brief Judge a point if it is in the map, never visited and reachable
 * @param p a point to be judged
 * @return true - in the map, never visited and reachable
 * @return false - otherwise
 */
bool Map::dfsPointValid(const point& p) {
	if (!(p.x >= 0 && p.x < MAP_ROWS)) {
		return false;
	} else if (!(p.y >= 0 && p.y < MAP_COLUMNS)) {
		return false;
	} else if (visited[p.x][p.y]) {
		return false;
	} else if (matrix[p.x][p.y] != 0) {
		return false;
	}
	return true;
}

/**
 * @fn Map::printPath(int length)
 * @brief Print the path to destination in the specific format
 * @param length the length of the path
 */
void Map::printPath(int length) {
	for (int i = 0; i < length; i++) {
		printf("(%d, %d)\n", path[i].x, path[i].y);
	}
	printf("\n\n");
}

/**
 * @fn Map::dfs(point cur, int step)
 * @brief Find a path to the destination via dfs
 * @param cur a point of where the car is now
 * @param step how many steps has the car searched
 * @return true  - can find such a path
 * @return false - otherwise
 * @pre call function initDfs()
 */
bool Map::dfs(point cur, int step) {
	if (cur == destination) {
		printPath(step);
		return true;
	} else {
		point p;
		for (int i = 0; i < 4; i++) {
			p.x = cur.x + oper_x[i];
			p.y = cur.y + oper_y[i];
			if (dfsPointValid(p)) {
				path[step] = p;
				p.step = cur.step + 1;
				visited[p.x][p.y] = true;
				if (dfs(p, step + 1)) {
					return true;
				}
			}
		}
	}
	return false;
}

/**
 * @fn Map::updateNextPosition()
 * @brief try to search for the path to the destination
 * @details If serach successfully, update the nextPosition,
 			else set the nextPosition unreachable
 */
void Map::updateNextPosition() {
	initDfs();
	if (dfs(currentPosition, 0)) {
		nextPosition = path[0];
	} else {
		printf("NO ROUTE!\n");
		nextPosition = INF_POINT;
	}
}

/**
 * @fn Map::nextPositionAvilable()
 * @brief check for the nextPosition if it is avialable
 * @return true  - reachable
 * @return false - unreachable
 */
bool Map::nextPositionAvilable() {
	return (matrix[nextPosition.x][nextPosition.y] == 0);
}

/**
 * @fn Map::getCurrentDirection()
 * @brief getter function of currentDirection
 * @return currentDirection
 */
direction Map::getCurrentDirection() {
	return currentDirection;
}

/**
 * @fn Map::setCurrentDirection(direction _currentDirection)
 * @brief setter function of currentDirection
 * @param _currentDirection a new currentDirection
 */
void Map::setCurrentDirection(direction _currentDirection) {
	currentDirection = _currentDirection;
}

/**
 * @fn Map::compareCurrent2nextDirection(char axis)
 * @brief compare current direction with next direction
 * @param axis a char symbol for the compare axis, 'x' or 'y'
 * @return 0  - equal
 * @return 1  - current > next
 * @return -1 - current < next
 */
int Map::compareCurrent2nextDirection(char axis) {
	if (axis == 'x') {
		if (currentPosition.x == nextPosition.x) {
			return 0;
		} else if (currentPosition.x > nextPosition.x) {
			return 1;
		} else {
			return -1;
		}
	} else {
		if (currentPosition.y == nextPosition.y) {
			return 0;
		} else if (currentPosition.y > nextPosition.y) {
			return 1;
		} else {
			return -1;
		}
	}
	return 0;
}

/**
  * @fn Map::printPosition(int index)
  * @brief Print three position in the specific format
  * @param index an int symbol
  				 1 - Current Position
  				 2 - Next Position
  				 3 - Destination
  */ 
void Map::printPosition(int index) {
	switch (index) {
		case 1:
			printf("Current Position: %d, %d\n", currentPosition.x, currentPosition.y);
			break;
		case 2:
			printf("Next Position: %d, %d\n", nextPosition.x, nextPosition.y);
			break;
		case 3:
			printf("Destination Position: %d, %d\n", destination.x, destination.y);
			break;
		default:
			break;
	}
}

/**
 * @fn Map::arriveDestination()
 * @brief Check if has reached the destination
 * @return true  - reached
 * @return false - otherwise
 */
bool Map::arriveDestination() {
	return (currentPosition == destination);
}

/**
 * @fn Map::getNextPosition()
 * @brief getter function of nextPosition
 * @return nextPosition
 */
point Map::getNextPosition() {
	return nextPosition;
}

/**
 * @fn Map::getCurrentPosition()
 * @brief getter function of currentPosition
 * @return currentPosition
 */
point Map::getCurrentPosition() {
	return currentPosition;
}

/**
 * @fn Map::getMapAtPosition(const int& position)
 * @brief getter function of the status of a specfic position in the map
 * @param position an int wich means the position you want formatted like 74
 * @return the status of the point
 */
int Map::getMapAtPosition(const int& position) {
	int row = position / MAP_ROWS;
	int col = position % MAP_COLUMNS;
	return matrix[row][col];
}

/**
 * @fn Map::setCurrentPosition(const point& p)
 * @brief setter function of currentPosition
 * @param p new currentPosition
 */
void Map::setCurrentPosition(const point& p) {
	currentPosition = p;
}

/**
 * @fn Map::setMapAtPosition(const point& p, const int& value)
 * @brief setter function of a position in the map
 * @param p the position which you want to set
 * @param value the status of the position which you want to set
 */
void Map::setMapAtPosition(const point& p, const int& value) {
	matrix[p.x][p.y] = value;
}

// ******************** point *********************************

/**
 * @fn point::point()
 * @brief Default constructor of point
 */
point::point() {}

/**
 * @fn point::point(int _x, int _y, int _step)
 * @brief Constructor of point with parameters
 * @param _x to set x
 * @param _y to set y
 * @param _step to set step
 */
point::point(int _x, int _y, int _step) {
	x = _x;
	y = _y;
	step = _step;
}

/**
 * @fn point::operator== (const point& other)
 * @brief overwritten function operator==
 * @param other a point which to be compared
 * @return true  - equals
 * @return false - unequals
 */
bool point::operator== (const point& other) {
	return (x == other.x && y == other.y);
}

/**
 * @fn point::operator!= (const point& other)
 * @brief overwritten function operator!=
 * @param other a point which to be compared
 * @return true  - unequals
 * @return false - equals
 */
bool point::operator!= (const point& other) {
	return (x != other.x || y != other.y);
}

/**
 * @fn point::toString()
 * @brief convert point to string in a specfic format
 * @return the string of result
 */
string point::toString() {
	string temp = Util::int2string(x * MAP_ROWS + y);
	return temp;
}