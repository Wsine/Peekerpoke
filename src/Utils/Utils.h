/*
 * Util.h
 *
 *  Created on: 2016��1��6��
 *      Author: liboyang
 */

#ifndef UTIL_H_
#define UTIL_H_

struct point  {
    int x;
    int y;
    int step;
};
#define EAST 1
#define WEST 2
#define SOUTH 3
#define NORTH 4
extern int Map[10][10];

#endif /* UTIL_H_ */
