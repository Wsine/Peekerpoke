/**
 * @file Car.h
 * @brief The declaration of class Car
 */

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
#include "Location.h"

using namespace std;

class Car {
private:
	string m_name;
	Map m_map;
	Motor m_motor;
	pthread_t pokeThread;
	pthread_t locationThread;
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
	friend void* startLocation(void *ptr);
	void forkThreadForLocation();
};

#endif