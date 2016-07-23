/*
 * Author : Cormac Guerin 
 *
 * Name: object.h
 * A class defining simulation objects in our world.
 * 
 * For a robot for example an object would represent a limb etc.
 *
 * The position and orientation is stored in Eigen Matrices
 * The corresponding physics body is also store here
 *
 * Essentially this is a state container for the body/object
 *
 */

#ifndef _OBJECT_NODE_H_
#define _OBJECT_NODE_H_

#include <iostream>
#include <Eigen/Dense>
#include "physics/physics_engine.h"
#include "model/mdl_parser.h"

class ObjectNode {
	private:
	public:

	int o_id;
	std::string name;
	bool activation_state;
	Eigen::Vector4f position;
	Eigen::Matrix4f orientation;
	PhysicsEngine* phybody;
	MdlParser::mdl_object modelobject;

};

#endif

