
#ifndef _PHYSICS_ENGINE_H_
#define _PHYSICS_ENGINE_H_

#include "Simbody.h"
#include "../model/mdl_parser.h"
//#include <bullet/btBulletDynamicsCommon.h>

class PhysicsEngine 
{
public:
	virtual SimTK::MobilizedBody getBody(){};
	virtual void createBox(MdlParser::mdl_object object, SimTK::MobilizedBody physicsBody){}
	virtual void createSphere(MdlParser::mdl_object object, SimTK::MobilizedBody physicsBody){}
	virtual void createCylinder(MdlParser::mdl_object object, SimTK::MobilizedBody physicsBody){}
	virtual void run(){}
	virtual void init(){}
	virtual ~PhysicsEngine(){};
private:
};

#endif

