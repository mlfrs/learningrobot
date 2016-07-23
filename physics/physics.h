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

class Physics 
{
private:

public:
	Physics(std::string engine);
	PhysicsEngine* phyEng;
	void createObject(MdlParser::mdl_object M, ObjectNode O);
	void run();
	~Physics();

};

#endif

