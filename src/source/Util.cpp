#include "Util.h"

Car* Util::car = 0;

/**
 * @fn Util::newCar(std::string s)
 * @brief always to construct a new car with its name
 * @param s the car's name
 * @return the reference of the new car
 */
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

/**
 * @fn Util::getCar()
 * @brief the only way to get access to the car
 * @details If car pointer is NULL, new one
 * @return the reference of the car
 */
Car& Util::getCar() {
	if (!car) {
		car = new Car();
	}
	return *car;
}

/**
 * @fn Util::int2string(int i)
 * @brief convert int to string
 * @param i int type
 * @return string result
 */
string Util::int2string(int i) {
	stringstream ss;
	string str;
	ss << i;
	ss >> str;
	return str;
}

/**
 * @fn Util::extractMapResult(std::string s)
 * @brief Extract the map status message from a string
 * @param s input string
 * @return the map status
 * @note input string must statistify "1/74/201623848.112/Car2"
 */
int Util::extractMapResult(std::string s) {
	int result = 0;
	stringstream ss;
	ss << s;
	ss >> result;
	return result;
}

/**
 * @fn Util::extractMapLocation(std::string s)
 * @brief Extract the map location message from a string
 * @param s input string
 * @return the map location
 * @note input string must statistify "1/74/201623848.112/Car2"
 */
point Util::extractMapLocation(std::string s) {
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
