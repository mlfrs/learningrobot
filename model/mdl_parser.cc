/*
 *
 * Name: MdlParser.cc
 *
 * Authors: Cormac Guerin
 *
 * Desc: Implentation file for MdlParser class. 
 *
 */

#include <stdio.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <cstdlib>

#include "mdl_parser.h"
#include "../ccfuncs.h"

using namespace std;

MdlParser::MdlParser() {}

void MdlParser::read(string model, 
	vector<mdl_object> &mdlobjects, 
	vector<mdl_joint> &mdljoints, 
	vector<mdl_function> &mdlfunctions, 
	vector<mdl_method> &mdlmethods) {

	bool alive = true;

	while (alive) {
		// Initiate the root value and reader.
		Json::Value root;
		Json::Reader reader;
		std::string filename = "store/" + model + ".mdl";
		std::ifstream mdlfile( filename.c_str(), std::ifstream::binary);

		// Check if the File exists and if we can actually parse it.
		if (mdlfile) { 
			cout << "mdl file " << filename << " found." << endl;
		} else {
			cout << "Warning : mdl file " << filename << " not found." << endl;
			break;
		}
		bool parsingSuccessful = reader.parse( mdlfile, root, false );
		if ( !parsingSuccessful ) {
			std::cout << reader.getFormattedErrorMessages() + "\n" << std::endl;
		}

		// Objects
		//
		// - Assign objects from root.
		//
		Json::Value objects = root["objects"];
		Json::Value::Members objectNames = root["objects"].getMemberNames();

		for(unsigned int o=0; o < objectNames.size(); o++) {
			
			mdl_object object;

			object.name = objectNames[o];
			object.type = objects[objectNames[o]]["type"].asString();
			object.physics_mesh = objects[objectNames[o]]["physics_mesh"].asString();
			object.graphics_mesh = objects[objectNames[o]]["graphics_mesh"].asString();
			object.graphics_material = objects[objectNames[o]]["graphics_material"].asString();
			object.physics_shape = objects[objectNames[o]]["physics_shape"].asString();
			object.light_type = objects[objectNames[o]]["light_type"].asString();

			std::string temp = objects[objectNames[o]]["mass"].asString();
			object.mass = ::atof(temp.c_str());

			const Json::Value s_dimension = objects[objectNames[o]]["dimension"];
			for ( int i = 0; i < s_dimension.size(); i++ ) {
				float temp = ::atof(s_dimension[i].asString().c_str());
				object.dimension[i] = temp;
			}

			const Json::Value s_rotation = objects[objectNames[o]]["rotation"];
			for ( int i = 0; i < s_rotation.size(); i++ ) {
				float temp = ::atof(s_rotation[i].asString().c_str());
				object.rotation[i] = temp;
			}

			const Json::Value s_position = objects[objectNames[o]]["position"];
			for ( int i = 0; i < s_position.size(); i++ ) {
				float temp = ::atof(s_position[i].asString().c_str());
				object.position[i] = temp;
			}

			const Json::Value s_angular = objects[objectNames[o]]["angular_dof"];
			for ( int i = 0; i < s_angular.size(); i++ ) {
				float temp = ::atof(s_angular[i].asString().c_str());
				object.angular[i] = temp;
			}

			const Json::Value s_diffuse = objects[objectNames[o]]["diffuse"];
			for ( int i = 0; i < s_diffuse.size(); i++ ) {
				float temp = ::atof(s_diffuse[i].asString().c_str());
				object.diffuse[i] = temp;
			}

			const Json::Value s_specular = objects[objectNames[o]]["specular"];
			for ( int i = 0; i < s_specular.size(); i++ ) {
				float temp = ::atof(s_specular[i].asString().c_str());
				object.specular[i] = temp;
			}

			// read object components, these form physical compound objects.
			Json::Value components = objects[objectNames[o]]["components"];
			Json::Value::Members componentNames = objects[objectNames[o]]["components"].getMemberNames();

			for(unsigned int c=0; c < componentNames.size(); ++c)
			{
				mdl_component component;

				const Json::Value s_dimension = components[componentNames[c]]["dimension"];
				for ( int i = 0; i < s_dimension.size(); i++ ) {
					float temp = ::atof(s_dimension[i].asString().c_str());
					component.dimension[i] = temp;
				}
	
				const Json::Value s_rotation = components[componentNames[c]]["rotation"];
				for ( int i = 0; i < s_rotation.size(); i++ ) {
					float temp = ::atof(s_rotation[i].asString().c_str());
					component.rotation[i] = temp;
				}
	
				const Json::Value s_position = components[componentNames[c]]["position"];
				for ( int i = 0; i < s_position.size(); i++ ) {
					float temp = ::atof(s_position[i].asString().c_str());
					component.position[i] = temp;
				}

				component.graphics_mesh = components[componentNames[c]].get("graphics_mesh","default").asString();
				component.graphics_material = components[componentNames[c]].get("graphics_material","default").asString();
				component.physics_mesh = components[componentNames[c]].get("physics_mesh","default").asString();
				component.physics_shape = components[componentNames[c]].get("physics_shape","default").asString();
				component.name = components[componentNames[c]]["name"].asString();

				object.component.push_back(component);
			}
			mdlobjects.push_back(object);
		}

		// joints(constraints)
		Json::Value joints = root["joints"];
		Json::Value::Members jointNames = root["joints"].getMemberNames();

		for(unsigned int j=0; j < jointNames.size(); j++) {
			
			mdl_joint joint;

			joint.name = jointNames[j];
			joint.joint_type = joints[jointNames[j]]["joint_type"].asString();
			joint.primary_object = joints[jointNames[j]]["primary_object"].asString();
			joint.secondary_object = joints[jointNames[j]]["secondary_object"].asString();

			const Json::Value s_primary_position = joints[jointNames[j]]["primary_position"];
			for ( int i = 0; i < s_primary_position.size(); i++ ) {
				float temp = ::atof(s_primary_position[i].asString().c_str());
				joint.primary_position[i] = temp;
			}
			const Json::Value s_secondary_position = joints[jointNames[j]]["secondary_position"];
			for ( int i = 0; i < s_secondary_position.size(); i++ ) {
				float temp = ::atof(s_secondary_position[i].asString().c_str());
				joint.secondary_position[i] = temp;
			}
			const Json::Value s_axis_p = joints[jointNames[j]]["primary_axis"];
			for ( int i = 0; i < s_axis_p.size(); i++ ) {
				float temp = ::atof(s_axis_p[i].asString().c_str());
				joint.primary_axis[i] = temp;
			}
			const Json::Value s_axis_s = joints[jointNames[j]]["secondary_axis"];
			for ( int i = 0; i < s_axis_s.size(); i++ ) {
				float temp = ::atof(s_axis_s[i].asString().c_str());
				joint.secondary_axis[i] = temp;
			}
			const Json::Value s_limit = joints[jointNames[j]]["suspension_limit"];
			for ( int i = 0; i < s_limit.size(); i++ ) {
				float temp = ::atof(s_limit[i].asString().c_str());
				joint.suspension_limit[i] = temp;
			}

			mdljoints.push_back(joint);
		}

		// functions
		Json::Value functions = root["functions"];
		Json::Value::Members functionNames = root["functions"].getMemberNames();
		for(unsigned int j=0; j < functionNames.size(); j++) {

			mdl_function function;
			function.name = functionNames[j];
			cout << "mdl functio ' " << function.name << endl;

			Json::Value force_type = functions[functionNames[j]]["force_type"];
			if (not force_type.isNull()) { function.force_type = force_type.asString(); }

			Json::Value parent = functions[functionNames[j]]["parent"];
			if (not parent.isNull()) { function.parent = parent.asString(); }

			vector<string> objects;
			const Json::Value a_object = functions[functionNames[j]]["act_on"];
			for ( int i = 0; i < a_object.size(); i++ ) {
				objects.push_back(a_object[i].asString());
			}
			function.objects = objects;

			vector<string> constraints;
			const Json::Value a_constraint = functions[functionNames[j]]["constraints"];
			for ( int i = 0; i < a_constraint.size(); i++ ) {
				constraints.push_back(a_constraint[i].asString());
			}
			function.constraints = constraints;

			Json::Value sforce = functions[functionNames[j]]["force"];
			if (not sforce.isNull()) { function.force = StringToFloat(sforce.asString()); }

			const Json::Value s_force_axis = functions[functionNames[j]]["force_axis"];
			for ( int i = 0; i < s_force_axis.size(); i++ ) {
				float temp = ::atof(s_force_axis[i].asString().c_str());
				function.axis[i] = temp;
			}
		
			const Json::Value s_force_pos = functions[functionNames[j]]["force_position"];
			for ( int i = 0; i < s_force_pos.size(); i++ ) {
				float temp = ::atof(s_force_pos[i].asString().c_str());
				function.position[i] = temp;
			}

			mdlfunctions.push_back(function);
		}

		// methods
		Json::Value methods = root["methods"];
		Json::Value::Members methodNames = root["methods"].getMemberNames();
		for(unsigned int j=0; j < methodNames.size(); j++) {
			
			mdl_method method;
			method.name = methodNames[j];

			Json::Value method_type = methods[methodNames[j]]["method_type"];
			if (not method_type.isNull()) { method.method_type = method_type.asString(); }

			vector<string> objects;
			const Json::Value a_object = methods[methodNames[j]]["act_on"];
			for ( int i = 0; i < a_object.size(); i++ ) {
				objects.push_back(a_object[i].asString());
			}
			method.objects = objects;

			Json::Value scomp = methods[methodNames[j]]["compensation"];
			if (not scomp.isNull()) { method.compensation = StringToFloat(scomp.asString()); }

			mdlmethods.push_back(method);
		}

		alive = false;
	}
}

void MdlParser::add(string mdl)
{
}

MdlParser::~MdlParser() {}

