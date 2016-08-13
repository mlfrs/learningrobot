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
	return phyEng->getStatefulObjectNode(objectNode);
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
	} else if (M.physics_shape == "ellipsoid") {
		std::cout << "Physics::createObject ellipsoid " << std::endl;
		phyEng->createEllipsoid(M,O);
	}
}

void Physics::createJoint(MdlParser::mdl_joint J, ObjectNode A, ObjectNode B) {
	if (J.joint_type == "free") {
		phyEng->createFreeJoint(J, A, B);
	} else if (J.joint_type == "ball") {
		phyEng->createBallJoint(J, A, B);
	}
}

void Physics::init() {
	phyEng->init();
}

void Physics::run() {
	phyEng->run();
}

Physics::~Physics() {
	std::cout << " Physics::~Physics() " << std::endl; 
	delete phyEng;
}

