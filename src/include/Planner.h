#ifndef PLANNER_H_
#define PLANNER_H_

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
#include "Utils.h"
using namespace std;

void DFS(point Start,  point End);
point GetNextPosition();
void MapInitial();
void print();
void Referesh(point Start,  point End);
void MapReferesh();
int IfNextValuable(point next);
#endif /* PLANNER_H_ */
