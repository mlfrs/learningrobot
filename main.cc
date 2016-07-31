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

int main(int argc, char* argv[]) {

	std::string model;

	// Parse arguments.
	if (argc==2) {
		model = argv[1];
	} else { 
		model = "chain";
	}
	std::cout << "model " << model << std::endl;

	// First class. Todo remove.
	someclass *myclass = new someclass(argc);

	// Instantiate physics engine.
	Physics *physics = new Physics("simbody");

	// Instantiate  Object Manager.
	ObjectManager *objectManager = new ObjectManager(*physics);

	// Try to load the model.
	try {
		objectManager->addMdl(model);
	} catch (const std::exception& e) {
		std::cout << "Error in adding Model : " << e.what() << std::endl;
		exit;
	}

	// Initialize the simulation.
	physics->init();

	// Main loop.
	while( 1 ) {
		physics->run();
		/*
		 * Here we can add other functions that can interface
		 * With the simulation, in this case getObject requests
		 * the physics engine to get the updated object state 
		 * and print out the position and rotation.
		 */
		objectManager->getObject(1);
		objectManager->getObject(2);
	}

	delete physics;
	delete objectManager;

	return 0;
}

