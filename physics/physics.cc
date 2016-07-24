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

void Physics::createObject(MdlParser::mdl_object M, ObjectNode O) {
	if (M.physics_shape == "box") {
		phyEng->createBox(M, phyEng->getBody());
	} else if (M.physics_shape == "sphere") {
		phyEng->createSphere(M, phyEng->getBody());
	} else if (M.physics_shape == "cylinder") {
		phyEng->createCylinder(M, phyEng->getBody());
	}
}

void Physics::run() {
	phyEng->init();
	phyEng->run();
}

Physics::~Physics() {
//	delete phyEng;
}

