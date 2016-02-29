#include <iostream>
#include <sstream>
#include <stdlib.h>
#include "Util.h"
using namespace std;

int main() {
	// Print the current map
	Util::getCar().getMap().printMap();
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
			std::string strNextPosition = Util::getCar().getMap().getNextPosition().toString();
			Util::getCar().startPeek(strNextPosition);
			// Print the current map
			Util::getCar().getMap().printMap();
			// Check if next position is avilable
			if (Util::getCar().getMap().nextPositionAvilable()) {
				Util::getCar().getMotor().goStraight();
			} else {
				Util::getCar().getMotor().stop();
				printf("Next Position is Locked, Waitting...\n");
			}
		}
	}
	printf("Program Exited.\n");
	return 0;
}