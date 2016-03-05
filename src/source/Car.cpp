/**
 * @file Car.cpp
 * @brief The implementation of class Car
 */

#include "Car.h"

/**
 * @brief Default constructor of class Car
 * @details Set the car name as "Car1" and fork a thread for poke
 */
Car::Car() {
	m_name = "Car1";
	forkThreadForPoke();
}

/**
 * @brief String constructor of class Car
 * @details Set the car name from param and fork a thread for poke
 * @param name A string of the car name
 */
Car::Car(string name) {
	m_name = name;
	forkThreadForPoke();
}

/**
 * @brief Default destructor of class Car
 * @details Cancel the thread of poke
 */
Car::~Car() {
	int cancelResult = pthread_cancel(pokeThread);
	if (s == 0) {
		printf("The Poke Thread was cancelled.\n");
	} else {
		printf("Cancel Poke Thread Failed...\n");
	}
}

/**
 * @brief Adjust the car direction according to the next positon
 */
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

/**
 * @brief getter function of m_map
 * @return m_map the same object of the map
 */
Map& Car::getMap() {
	return m_map;
}

/**
 * @brief getter function of m_motor
 * @return m_motor the same object of the motor
 */
Motor& Car::getMotor() {
	return m_motor;
}

/**
 * @brief New a poke class and start to do poke
 * @details Set the thread cancelable, format the poke text, judge if data is sent
 * @return NULL
 * @note onCall by the function forkThreadForPoke()
 * @note ptr is a pointer pointed to a c-string
 */
void* startPoke(void *ptr) {
	int canCancel = pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
	if (canCancel != 0) {
		printf("This Poke Thread can not be cancelled.\n");
	}

	std::string carName((char*)ptr);
	Poke poke(carName);
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

/**
 * @brief New a peek class and start to do peek
 * @details Format the peek text, Set the params of peek, judeg if data is received
 * @param ptr A string of what to be sent out
 */
void Car::startPeek(std::string ptr) {
	const std::string ndnPrefix = "ndn:/place/"; 
	Peek peek(m_name);
	peek.setMustBeFresh();
	peek.setPayloadOnly();
	peek.setTimeout(3);
	peek.setPrefixName(ndnPrefix + ptr);
	peek.run();
	if (peek.isDataReceived()) {
		std::string receivedData = peek.getReceivedData();
		point location = Util::extractMapLocation(receivedData);
		int result = Util::extractMapResult(receivedData);
		m_map.setMapAtPosition(location, result);
	}
	else {
		printf("Data Received Failed.\n");
	}
}

/**
 * @brief Fork a sub thread for poke
 */
void Car::forkThreadForPoke() {
	int createThreadResult;
	createThreadResult = pthread_create(&pokeThread, NULL, startPoke, (void*)m_name.c_str());
	if (createThreadResult) {
		printf("Try Create Sub Thread Failed.\n");
	} else {
		printf("Sub Thread Started.\n");
	}
}

