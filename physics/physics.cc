/*
 * Authors: Cormac Guerin
 *
 * Name: physics.cc
 *
 * The physics wrapper class for generic functions
 * Different physics engines ie. bullet/phyEng can 
 * be initialized and called here
 *
 */

#include "physics.h"
#include "mlfrs_simbody.h"
#include <iostream>

Physics::Physics(std::string engine) {
	if (engine=="simbody") {
		phyEng = new MlfrsSimbody();
	} else if (engine=="bullet") {
		// todo
	} else {
		std::cout << "physics.cc - no engine supplied in initializer. bailing..";
		std::exit(0);
	}
}

ObjectNode Physics::getObjectNode(ObjectNode objectNode) {
	phyEng->getStatefulObjectNode(objectNode);
}

void Physics::createObject(MdlParser::mdl_object M, ObjectNode O) {
	if (M.physics_shape == "box") {
		std::cout << "Physics::createObject box " << std::endl;
		phyEng->createBox(M,O);
	} else if (M.physics_shape == "sphere") {
		std::cout << "Physics::createObject sphere " << std::endl;
		phyEng->createSphere(M,O);
	} else if (M.physics_shape == "cylinder") {
		std::cout << "Physics::createObject cylinder " << std::endl;
		phyEng->createCylinder(M,O);
	}
}

void Physics::run() {
	phyEng->run();
}

Physics::~Physics() {
//	delete phyEng;
}

