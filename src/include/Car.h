#ifndef CAR_H_
#define CAR_H_

#include <string.h>
#include "Map.h"
#include "Motor.h"

class Car {
private:
	String m_name;
	Map m_map;
	Motor m_motor;
/* Method */
public:
	Car(String name);
	~Car();
	void adjust_direction();
};

#endif