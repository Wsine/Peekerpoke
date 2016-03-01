#include "Map.h"
#include "Util.h"

Map::Map() {
	initial();
}

Map::~Map() {}

void Map::initMatrix() {
	/* set the whole map obstacle */
	memset(matrix, 1, sizeof(matrix));
	/* set the avilable positions */
	for (int i = 0; i < MAP_ROWS; i++) {
		matrix[i][4] = 0;
	}
	for (int i = 0; i < MAP_COLUMNS; i++) {
		matrix[4][i] = 0;
	}
}

void Map::initDestination() {
	destination = point(9, 4);
}

void Map::initCurrentPosition() {
	currentPosition = point(0, 4);
}

void Map::initCurrentDirection() {
	currentDirection = d_east;
}

void Map::initial() {
	initMatrix();
	initDestination();
	initCurrentPosition();
	initCurrentDirection();
}

void Map::printMap() {
	printf("Current map:\n");
	for (int i = 0; i < MAP_ROWS; i++) {
		for (int j = 0; j < MAP_COLUMNS; j++) {
			printf("%d \n", matrix[i][j]);
		}
		printf("\n");
	}
	printf("\n\n");
}

void Map::initDfs() {
	memset(visited, false, sizeof(visited));
	visited[currentPosition.x][currentPosition.y] = true;
	currentPosition.step = 1;
}

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

void Map::printPath(int length) {
	for (int i = 0; i < length; i++) {
		printf("(%d, %d)\n", path[i].x, path[i].y);
	}
	printf("\n\n");
}

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

void Map::updateNextPosition() {
	initDfs();
	if (dfs(currentPosition, 0)) {
		nextPosition = path[0];
	} else {
		printf("NO ROUTE!\n");
		nextPosition = point(0, 0);
	}
}

bool Map::nextPositionAvilable() {
	return (matrix[nextPosition.x][nextPosition.y] == 0);
}

direction Map::getCurrentDirection() {
	return currentDirection;
}

void Map::setCurrentDirection(direction _currentDirection) {
	currentDirection = _currentDirection;
}

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

bool Map::arriveDestination() {
	return (currentPosition == destination);
}

point Map::getNextPosition() {
	return nextPosition;
}

point Map::getCurrentPosition() {
	point temp;
	printf("Please input current position: ");
	std::cin >> temp.x >> temp.y;
	currentPosition.x = temp.x;
	currentPosition.y = temp.y;
	return currentPosition;
}

int Map::getMapAtPosition(const int& position) {
	int row = position / MAP_ROWS;
	int col = position % MAP_COLUMNS;
	return matrix[row][col];
}

string point::toString() {
	string temp = Util::int2string(x * MAP_ROWS + y);
	return temp;
}