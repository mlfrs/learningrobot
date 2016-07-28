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

int main(int argc, char* argv[])
{

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

	// Run/Step the simulation.
	physics->run();
	objectManager->getObject(2);

//	delete physics;
//	delete objectManager;

	return 0;
}

