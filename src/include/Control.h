#ifndef CONTROL_H_
#define CONTROL_H_
#include <iostream>
#include <termios.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include "Utils.h"
using namespace std;

bool IfControlWork();
int set_interface_attribs (int fd, int speed, int parity);
void set_blocking (int fd, int should_block) ;
void AdjustDirection(point CurrentPosition,  point NextPosition,  int &CurrentDirection);
void Turn180();
void TurnLeft();
void TurnRight();
void GoStraight();
void Stop();
#endif /* CONTROL_H_ */
