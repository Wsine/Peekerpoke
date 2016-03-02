#include "Car.h"

Car::Car() {
	m_name = "Car1";
	forkThreadForPoke();
}

Car::Car(string name) {
	m_name = name;
	forkThreadForPoke();
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

void* startPoke(void *ptr) {
	Poke poke;
	char *preFixName = "ndn:/place";
	poke.setPrefixName(preFixName);
	poke.run();
	if (poke.isDataSent()) {
		printf("Data Sent Successfully.\n");
	} else {
		printf("Data Sent Failed.\n");
	}
	printf("Sub Thread Exited.\n");
	return NULL;
}

void Car::startPeek(std::string ptr) {
	const std::string ndnPrefix = "ndn:/place/"; 
	Peek peek;
	peek.setMustBeFresh();
	peek.setPayloadOnly();
	peek.setTimeout(3);
	peek.setPrefixName(ndnPrefix + ptr);
	peek.run();
	if (peek.isDataReceived()) {
		printf("Received Data Successfully.\n");
		std::string receivedData = peek.getReceivedData();
		printf("receivedData = %s\n", receivedData.c_str());
	}
	else {
		printf("Data Received Failed.\n");
	}
}

void Car::forkThreadForPoke() {
	pthread_t thread;
	int createThreadResult;
	createThreadResult = pthread_create(&thread, NULL, startPoke, NULL);
	if (createThreadResult) {
		printf("Try Create Sub Thread Failed.\n");
	} else {
		printf("Sub Thread Started.\n");
		
	}
}

