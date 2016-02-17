/*
 * main.cpp
 *
 *  Created on: 2015.12.30
 *  Author: liboyang
 */
#include <iostream>
#include <sstream>
#include <stdlib.h>
#include "Control.h"
#include "Util.h"
#include "Planner.h"
using namespace std;


int main()  {

	point End;
	point NextPosition;
	point CurrentPosition;
	int CurrentDirection;
	
	MapInitial();
	cout << "End:";
	cin >> End.x >> End.y;
	cout << "Initial direction(1-4):";
	cin >> CurrentDirection;
	print();
	while(1)  {
		cout << "Current Position: ";
		cin >> CurrentPosition.x >> CurrentPosition.y;
		Referesh(CurrentPosition,  End);
		DFS(CurrentPosition,  End);
		NextPosition = GetNextPosition();
		cout << "Next Position: " << NextPosition.x << ", " << NextPosition.y << endl;		
		if  (CurrentPosition.x == End.x && CurrentPosition.y == End.y)  {
			cout << "Arrived!" << endl << endl;
			Stop();
			break;
		}
		else  {
			AdjustDirection(CurrentPosition,  NextPosition,  CurrentDirection);
			// communication
			string m_interest = "";
			int interest_loc = NextPosition.x * 10 + NextPosition.y;
			stringstream ss;
			ss << interest_loc;
			ss >> m_interest;
			string temp = "sudo /home/ubuntu/ndn-tools/build/bin/ndnpeek ndn:/place/" + m_interest + " -f -p -w 3";
			const char* str = temp.c_str();
			cout << str << endl;
			system(str);
			MapReferesh();
			print();
			int result = IfNextValuable(NextPosition);
			if (result == 1) {
				GoStraight();
			}
			else {
				Stop();
				cout << "Next position is locked, waiting....." << endl;
			}
			
		}
	}
	cout << "Program is finished" << endl;
	//system("pause");
	return 0;
}

