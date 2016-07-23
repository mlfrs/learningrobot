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
	// First class. Todo remove.
	someclass *myclass = new someclass(argc);

	// Instantiate physics engine.
	Physics *physics = new Physics("simbody");

	// Instantiate  Object Manager.
	ObjectManager *objectManager = new ObjectManager(*physics);

	// Try to load the model.
	try {
		objectManager->addMdl("robot");
	} catch (const std::exception& e) {
		std::cout << "Error in adding Model : " << e.what() << std::endl;
		exit;
	}

	// Run/Step the simulation.
	physics->run();

	delete physics;
	delete objectManager;

	return 0;
}

