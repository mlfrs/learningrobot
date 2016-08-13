
#ifndef _PHYSICS_ENGINE_H_
#define _PHYSICS_ENGINE_H_

#include "Simbody.h"
#include "../object_node.h"
#include "../model/mdl_parser.h"
//#include <bullet/btBulletDynamicsCommon.h>

class PhysicsEngine 
{
public:
//	virtual SimTK::MobilizedBody getBody(){};
	PhysicsEngine(){};
	virtual void createBox(MdlParser::mdl_object object, ObjectNode objectNode){}
	virtual void createSphere(MdlParser::mdl_object object, ObjectNode objectNode){}
	virtual void createCylinder(MdlParser::mdl_object object, ObjectNode objectNode){}
	virtual void createEllipsoid(MdlParser::mdl_object object, ObjectNode objectNode){}
	virtual void run(){}
	virtual void init(){}
	virtual ObjectNode getStatefulObjectNode(ObjectNode O){}
	virtual ObjectNode buildObjectNode(ObjectNode O, SimTK::MobilizedBody mBody){}
	virtual void createBallJoint(MdlParser::mdl_joint J, ObjectNode A, ObjectNode B){}
	virtual void createFreeJoint(MdlParser::mdl_joint J, ObjectNode A, ObjectNode B){}
	virtual ~PhysicsEngine(){};
private:
};
#endif

