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
#include "model/mdl_parser.h"

class ObjectNode {
	private:
	public:

	int oId;
	std::string name;
	bool activation_state;
	Eigen::Vector3f position;
	Eigen::Vector4f rotation; 
//	Eigen::Matrix3f rotation;
	MdlParser::mdl_object modelobject;
};

#endif

