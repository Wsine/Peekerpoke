#include "../include/Map.h"

Map::Map() {}

Map::~Map() {}

void Map::initial() {

}

void Map::print() {

}

void Map::set_matrix() {
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

void Map::set_destination() {

}

point Map::get_next_position() {

}
