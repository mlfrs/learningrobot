//
// MLFRS (Machine Learning for Robotic simulations)
//
// Authors : Cormac Guerin, Tom Stewart.
//

#include "someclass.h"
#include "physics/physics.h"
#include "object_manager.h"
#include "object_node.h"
#include <stdio.h>
#include <iostream>

int main(int argc, char** argv)
{
	someclass *myclass = new someclass(argc);
	Physics *phy = new Physics();
	ObjectManager *objectManager = new ObjectManager();
	objectManager->addMdl("robot");
	phy->init();

	return 0 ;
}

