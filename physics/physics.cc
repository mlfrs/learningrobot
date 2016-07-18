/*
 * Authors: Cormac Guerin
 *
 * Name: physics.cc
 *
 * The physics wrapper class for generic functions
 * Different physics engines ie. bullet/simbody can 
 * be initialized and called here
 *
 */

#include "physics.h"
#include <iostream>

Physics::Physics()
{
}

void Physics::init() {
//	simbody.visualize();
	simbody.realize();
	simbody.run();
}

void Physics::createObject(MdlParser::mdl_object M, ObjectNode O) {
	if (M.physics_shape == "box") {
//		createBox(M, O, B);
	} else if (M.physics_shape == "sphere") {
		simbody.createSphere(M, O.phybody);
	} else if (M.physics_shape == "cylinder") {
//		createCylinder(M, O, B);
	}
}

Physics::~Physics() {}

