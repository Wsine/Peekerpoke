#ifndef UTIL_H_
#define UTIL_H_

#include <string.h>
#include <sstream>
#include "Car.h"

class Car; // Fix in Linux; Stupid!!!

class Util {
private:
	static Car* car;
public:
	static Car& getCar();
	static std::string int2string(int i);
};

#endif