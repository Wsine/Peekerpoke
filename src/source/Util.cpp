#include "../include/Util.h"

Car* Util::car = 0;
Car& Util::getCar() {
	if (!car) {
		car = new Car();
	}
	return *car;
}

string Util::int2string(int i) {
	stringstream ss;
	string str;
	ss << i;
	ss >> str;
	return str;
}