/*
 *
 * Name: model.h
 *
 * Author: Cormac Guerin
 *
 * Desc: Header file for model class. 
 *
 */ 


#ifndef _MDLPARSER_H_
#define _MDLPARSER_H_

#include <string>
#include <jsoncpp/json/json.h>

class MdlParser {

	private:

	public:
	MdlParser( );

	struct mdl_component {
		int id;
		std::string name;
		std::string graphics_mesh;
		std::string graphics_material;
		std::string physics_shape;
		std::string physics_mesh;
		float dimension[3];
		float rotation[4];
		float position[3];
	};

	struct mdl_object {
		int id;
		std::string name;
		std::string type;
		std::string graphics_mesh;
		std::string graphics_material;
		std::string physics_shape;
		std::string physics_mesh;
		std::string light_type;
		float mass;
		float dimension[3];
		float rotation[4];
		float position[3];
		float angular[3];
		float diffuse[3];
		float specular[3];
		std::vector<mdl_component> component;
	};

	struct mdl_joint {
		std::string name;
		std::string joint_type;
		std::string primary_object;
		std::string secondary_object;
		float primary_position[3];
		float secondary_position[3];
		float axis[3];
		float primary_axis[3];
		float secondary_axis[3];
		float steer_limit;
		float suspension_limit[2];
		float suspension_stiffness;
		float suspension_equilibrium;
	};

	struct mdl_function {
		std::string name;
		std::string force_type;
		std::string act_on;
		std::string parent;
		float axis[3];
		float position[3];
		float force;
		std::vector<std::string> objects;
		std::vector<std::string> constraints;
	};

	struct mdl_method {
		std::string name;
		std::string method_type;
		std::string act_on;
		float compensation;
		float rotation[4];
		std::vector<std::string> objects;
	};
	
	void read(std::string model, std::vector<mdl_object> &mdlobjects, std::vector<mdl_joint> &mdljoints, std::vector<mdl_function> &mdlfunctions, std::vector<mdl_method> &mdlmethods);

	void add(std::string mdlfile);

	~MdlParser();
};

#endif
