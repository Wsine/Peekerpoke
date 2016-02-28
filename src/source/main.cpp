#include <iostream>
#include <sstream>
#include <stdlib.h>
#include "../include/Util.h"
using namespace std;

int main() {
	// Print the current map
	Util::getCar().get_map().print_map();
	while (true) {
		// Print current position
		Util::getCar().get_map().print_position(1);
		// Update next position
		Util::getCar().get_map().update_next_position();
		// Print next position
		Util::getCar().get_map().print_position(2);
		// Check if arrive destination
		if (Util::getCar().get_map().arrive_destination()) {
			printf("Arrived Destination\n");
			Util::getCar().get_motor().stop();
			break;
		} else {
			// Adjust the direction based current2next position
			Util::getCar().adjust_direction();
			/* Communication */
			// TODO: refactor the communication part
			string m_interest = "";
			point NextPosition = Util::getCar().get_map().get_next_position();
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
			Util::getCar().get_map().print_map();
			// Check if next position is avilable
			if (Util::getCar().get_map().next_position_avilable()) {
				Util::getCar().get_motor().go_straight();
			} else {
				Util::getCar().get_motor().stop();
				printf("Next Position is Locked, Waitting...\n");
			}
		}
	}
	printf("Program Exited.\n");
	return 0;
}