#include "Util.h"

Car* Util::car = 0;
Car& Util::newCar(std::string s) {
	// Remerber: only called once
	if (!car) {
		car = new Car(s);
	} else {
		delete car;
		car = new Car(s);
	}
	return *car;
}

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

int Util::extractMapResult(std::string s) {
	// s must satistify "1/74/201623848.112/Car2"
	int result = 0;
	stringstream ss;
	ss << s;
	ss >> result;
	return result;
}

point Util::extractMapLocation(std::string s) {
	// s must satistify "1/74/201623848.112/Car2"
	point p;
	int location;
	char c;
	stringstream ss;
	ss << s;
	ss >> c >> c;
	ss >> location;
	p.x = location / MAP_ROWS;
	p.y = location % MAP_COLUMNS;
	return p;
}
