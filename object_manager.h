/*
 *
 * Authors: Cormac Guerin
 *
 * Name: object_manager.h
 * 
 * A class for managing  objects. load, save etc.
 *
 */

#ifndef _OBJECT_MANAGER_H_
#define _OBJECT_MANAGER_H_

#include "physics/physics.h"

class ObjectManager {

public:
	ObjectManager(const Physics& physics);

	void addMdl(std::string model);
	ObjectNode createObject(MdlParser::mdl_object object);
	void createJoint(MdlParser::mdl_joint joint,int oid_a, int oid_b);
	void togglePhysicsVisibility();

	~ObjectManager();

private:
	Physics physics;
};

#endif
