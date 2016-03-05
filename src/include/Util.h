/**
 * @file Util.h
 * @brief The declaration of class Util
 */

#ifndef UTIL_H_
#define UTIL_H_

#include <string.h>
#include <sstream>
#include "Map.h"
#include "Car.h"

/**
 * Fix in Linux; Stupid!!! Ignore me!
 */
class Car; // Fix in Linux; Stupid!!!

class Util {
private:
	static Car* car;
public:
	static Car& newCar(std::string s);
	static Car& getCar();
	static std::string int2string(int i);
	static int extractMapResult(std::string s);
	static point extractMapLocation(std::string s);
};

#endif