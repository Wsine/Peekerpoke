#include "../include/Car.h"

Car::Car() {
	m_name = "Car No.1";
	thread t(startPoke);
}

Car::Car(string name) {
	m_name = name;
	thread t(startPoke);
}

Car::~Car() {}

void Car::adjustDirection() {
	if (m_map.compareCurrent2nextDirection('x') == 0) {
		if (m_map.compareCurrent2nextDirection('y') == -1) {
			switch (m_map.getCurrentDirection()) {
				case d_east:
					break;
				case d_west:
					m_motor.turn180();
					break;
				case d_south:
					m_motor.turnLeft();
					break;
				case d_north:
					m_motor.turnRight();
					break;
				default:
					break;
			}
			m_map.setCurrentDirection(d_east);
			printf("Current direction: EAST\n");
		} else {
			switch (m_map.getCurrentDirection()) {
				case d_east:
					m_motor.turn180();
					break;
				case d_west:
					break;
				case d_south:
					m_motor.turnRight();
					break;
				case d_north:
					m_motor.turnLeft();
					break;
				default:
					break;
			}
			m_map.setCurrentDirection(d_west);
			printf("Current direction: WEST\n");
		}
	} else if (m_map.compareCurrent2nextDirection('y') == 0) {
		if (m_map.compareCurrent2nextDirection('x') == -1) {
			switch (m_map.getCurrentDirection()) {
				case d_east:
					m_motor.turnRight();
					break;
				case d_west:
					m_motor.turnLeft();
					break;
				case d_south:
					break;
				case d_north:
					m_motor.turn180();
					break;
				default:
					break;
			}
			m_map.setCurrentDirection(d_south);
			printf("Current direction: SOUTH\n");
		} else {
			switch (m_map.getCurrentDirection()) {
				case d_east:
					m_motor.turnLeft();
					break;
				case d_west:
					m_motor.turnRight();
					break;
				case d_south:
					m_motor.turn180();
					break;
				case d_north:
					break;
				default:
					break;
			}
			m_map.setCurrentDirection(d_north);
			printf("Current direction: NOTRH\n");
		}
	}
}

Map& Car::getMap() {
	return m_map;
}

Motor& Car::getMotor() {
	return m_motor;
}

void Car::startPoke() {
	Poke poke;
	poke.setPrefixName("ndn:/place");
	poke.run();
	if (poke.isDataSent()) {
		printf("Data Sent Successfully.\n");
	} else {
		printf("Data Sent Failed.\n");
	}
	printf("Sub Thread Exited.\n");
}