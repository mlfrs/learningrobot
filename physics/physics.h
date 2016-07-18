/*
 * Authors: Cormac Guerin
 *
 * Name: physics.h
 *
 */

#ifndef _PHYSICS_H_
#define _PHYSICS_H_

#include "mlfrs_simbody.h"
#include "physics_body.h"
#include "../model/mdl_parser.h"
#include "../object_node.h"

#include <iostream>

class Physics 
{
private:

public:
	Physics();

	MlfrsSimbody simbody;

	void init();
	void createObject(MdlParser::mdl_object M, ObjectNode O);
	~Physics();

};

#endif

