#ifndef CAR_H_
#define CAR_H_

#include <stdlib.h>
#include <string.h>
#include <sstream>
#include <iostream>
#include <thread.h>
#include "Map.h"
#include "Motor.h"
#include "Poke.h"
using namespace std;

class Car {
private:
	string m_name;
	Map m_map;
	Motor m_motor;
/* Method */
public:
	Car();
	Car(string name);
	~Car();
	void adjustDirection();
	Map& getMap();
	Motor& getMotor();
	void startPoke();
};

#endif