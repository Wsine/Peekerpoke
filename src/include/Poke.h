#include "../../lib/ndn-tools/version.hpp"
#include <fstream>
#include <sstream>
#include "time.h"
#include <stdio.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>

class Poke
{
public:
	Poke();
	~Poke();
	
};