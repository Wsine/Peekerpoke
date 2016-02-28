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
			/* Communication */
			// TODO: refactor the communication part
			string m_interest = "";
			point NextPosition = Util::getCar().getMap().getNextPosition();
			int interest_loc = NextPosition.x * 10 + NextPosition.y;
			stringstream ss;
			ss << interest_loc;
			ss >> m_interest;
			string temp = "sudo /home/ubuntu/ndn-tools/build/bin/ndnpeek ndn:/place/" + m_interest + " -f -p -w 3";
			const char* str = temp.c_str();
			cout << str << endl;
			system(str);
			/*****************/
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