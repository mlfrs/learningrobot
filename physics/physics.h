/*
 * Authors: Cormac Guerin
 *
 * Name: physics.h
 *
 */

#ifndef _PHYSICS_H_
#define _PHYSICS_H_

#include "physics_engine.h"
#include "../model/mdl_parser.h"
#include "../object_node.h"
#include <iostream>

#include "mlfrs_simbody.h"

class Physics 
{
private:
	PhysicsEngine* phyEng;

public:
	Physics(std::string engine);
	ObjectNode getObjectNode(ObjectNode objectNode);
	void createObject(MdlParser::mdl_object M, ObjectNode O);
	void run();
	~Physics();

};

#endif

