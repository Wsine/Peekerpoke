#ifndef UTIL_H_
#define UTIL_H_

#include <string.h>
#include <sstream>
#include "../include/Car.h"
using namespcae std;

class Util {
private:
	static Car* car;
public:
	static Car& getCar();
	static string int2string(int i);
};

#endif