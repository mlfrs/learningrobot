/*
 * Name: object.cc
 *
 * Authors: Cormac Guerin
 * 
 * - All rights reserved.
 *
 * This class provides a common interface to creating objects.
 * In principle every object created has an object id (o_id). 
 * physics instance (ie. rigidBody) !
 *
 * This class also contains a function for loading a mdl file
 * into the engine, this uses the mdlparser to break the mdl
 * into separate objects as described above.
 *
 * All objects created in the world, should be created via this
 * class to ensure continuity.
 *
 * Todo: Currently this is very non threadsafe there should be
 * a more threadsafe method.
 *
 */

#include <stdio.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>

#include "object_node.h"
#include "model/model.h"
#include "model/mdl_parser.h"
#include "object_node.h"
#include "object_manager.h"
#include "physics/physics.h"

using namespace std;

ObjectManager::ObjectManager() {}

int m_id = 0;
int o_id = 0;
int c_id = 0;
int p_id = 0;

Physics physics;
MdlParser mdl;

std::vector<typename Model::Model> models;
std::vector<typename ObjectNode::ObjectNode> objects;

void ObjectManager::addMdl(string mdlfile) {

	int base_oid=o_id;
	int base_cid=c_id;
	Model gmodel;
	ObjectNode tmp_object;

	vector<ObjectNode> gobject;
	vector<MdlParser::mdl_object> modelObject;
	vector<MdlParser::mdl_joint> joint;
	vector<MdlParser::mdl_function> function;
	vector<MdlParser::mdl_method> method;
	mdl.read(mdlfile, modelObject, joint, function, method);

	if (!modelObject.empty()) {
		int o_id;
		for (int i = 0; i < modelObject.size(); i++) {
// 	  	 if we have some separate graphics engine, can load here.
//		    if ( object[i].component.size() > 1 ) {
//				gfxNode.createCompoundObjectNode(object[i], N);
//			} else {
//				gfxNode.createObject(object[i], N);
//			}
			tmp_object = createObject(modelObject[i]);
			gobject.push_back(tmp_object);
		}

		int oid_a,oid_b;
		// loop through all joints and find the objects they correspond to.
		for (int j = 0; j < joint.size(); j++) {
			for (int i = 0; i < modelObject.size(); i++) {
				if ( modelObject[i].name.compare(joint[j].primary_object) == 0 ) {
					oid_a=i;
				} else if ( modelObject[i].name.compare(joint[j].secondary_object) == 0 ) {
					oid_b=i;
				}
			}
//			createJoint(joint[j],modelObject.size()-oid_a, modelObject.size()-oid_b);
		}
	}
	gmodel.objects = gobject;
	gmodel.model_id = m_id;
	models.push_back(gmodel);
	m_id++;

}

ObjectNode ObjectManager::createObject(MdlParser::mdl_object modelObject) {

	ObjectNode object;
	PhysicsBody* phybody;// = new SimbodyPhysics;

	physics.createObject(modelObject, object);

	object.o_id = o_id;
	object.phybody = phybody;
	object.modelobject = modelObject;

	o_id++;
	return object;

}

//void ObjectManager::createJoint(MdlParser::mdl_joint joint, int oid_a, int oid_b) {
//	physics.createJoint(joint, o_id-oid_a, o_id-oid_b);
//}

void ObjectManager::togglePhysicsVisibility(){
//	gfxNode.togglePhysicsVisibility();
}

ObjectManager::~ObjectManager() {}
