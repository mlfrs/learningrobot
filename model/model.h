/*
 * Name: object.h
 *
 * Author: Cormac Guerin.
 *
 * A class defining objects in our world.
 * An object should have enough information at any time 
 * to be able to destroy/recreate/duplicate the object.
 *
 */

#ifndef _MODEL_H_
#define _MODEL_H_

#include <vector>

class Model {
	private:
	public:

	int model_id;
	bool activation_state;
	std::string name;
	std::string type;
	std::vector<ObjectNode> objects;

};

#endif

