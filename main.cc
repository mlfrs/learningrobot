//
// MLFRS (Machine Learning for Robotic simulations)
//

#include "someclass.h"
#include "physics/physics.h"
#include <stdio.h>

int main(int argc, char** argv)
{
	someclass *myclass = new someclass(argc);
	physics *phy = new physics();

	phy->init();

	return 0 ;
}

