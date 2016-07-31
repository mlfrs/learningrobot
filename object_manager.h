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
	ObjectManager(Physics& physics);

	void addMdl(std::string model);
	void createJoint(MdlParser::mdl_joint joint,int oid_a, int oid_b);
	void togglePhysicsVisibility();
	void getObject(int oid, bool update = true);

	~ObjectManager();

private:
	Physics physics;
	int m_id = 0;
	int oId = 0;
	int c_id = 0;
	int p_id = 0;
};

#endif
