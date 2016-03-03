#include <iostream>
#include <sstream>
#include <stdlib.h>
#include "Map.h"
#include "Util.h"
using namespace std;

int main() {
	// Construct a new car with a name
	Util::newCar("Car2");
	// Print the current map
	Util::getCar().getMap().printMap();
	std::string strNextPosition = "";
	while (true) {
		// Print current position
		Util::getCar().getMap().printPosition(1);
		// Update next position
		Util::getCar().getMap().updateNextPosition();
		// Print next position
		Util::getCar().getMap().printPosition(2);
		// Check if arrive destination
		if (Util::getCar().getMap().arriveDestination()) {
			printf("Arrived Destination\n");
			Util::getCar().getMotor().stop();
			break;
		} else {
			// Adjust the direction based current2next position
			Util::getCar().adjustDirection();
			// Communication to ask for the status of next position.
			point nextPosition = Util::getCar().getMap().getNextPosition();
			if (nextPosition != INF_POINT) {
				// Can get the road
				strNextPosition = nextPosition.toString();
				Util::getCar().startPeek(strNextPosition);
			} else if (strNextPosition == "") {
				// There is no way to get there
				printf("Can not get the road to the destination at the beginning.\n");
				break;
			} else {
				// Peek to ask for the previous next position
				Util::getCar().startPeek(strNextPosition);
			}
			// Print the current map
			Util::getCar().getMap().printMap();
			// Check if next position is avilable
			if (Util::getCar().getMap().nextPositionAvilable()) {
				// TODO: Update current positon via NFC Module
				// Util::getCar().getMotor().goStraight();

				// Set to auto-incremented current positon by x
				point newCurrentPosition = Util::getCar().getMap().getCurrentPosition();
				newCurrentPosition = point(newCurrentPosition.x + 1, newCurrentPosition.y);
				Util::getCar().getMap().setCurrentPosition(newCurrentPosition);
			} else {
				Util::getCar().getMotor().stop();
				printf("Next Position is Locked, Waitting...\n");
			}
		}
	}
	printf("Program Exited.\n");
	return 0;
}