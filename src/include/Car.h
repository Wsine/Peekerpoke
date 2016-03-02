#ifndef CAR_H_
#define CAR_H_

#include <stdlib.h>
#include <string.h>
#include <sstream>
#include <iostream>
#include <pthread.h>
#include "Map.h"
#include "Motor.h"
#include "Poke.h"
#include "Peek.h"
using namespace std;

//class Map; // Fix in Linux; Stupid!!!

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
	friend void* startPoke(void *ptr);
	void startPeek(std::string ptr);
	void forkThreadForPoke();
};

#endif