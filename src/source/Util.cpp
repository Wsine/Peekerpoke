#include "../include/Util.h"

string Util::int2string(int i) {
	stringstream ss;
	string str;
	ss << i;
	ss >> str;
	return str;
}