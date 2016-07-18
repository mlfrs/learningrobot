
#ifndef _PHYSICS_BODY_H_
#define _PHYSICS_BODY_H_

#include "Simbody.h"
//#include <bullet/btBulletDynamicsCommon.h>

// template <class T>
class PhysicsBody 
{
private:

public:
	PhysicsBody();
	SimTK::Body::Rigid getSimbody() {
//		SimTK::Body::Rigid simBody;
		SimTK::Body::Rigid bodyInfo(SimTK::MassProperties(1.0, SimTK::Vec3(0), SimTK::UnitInertia(1)));
		return bodyInfo;
	}
};

//PhysicsBody::PhysicsBody () {
//	engine = s;
//	if (engine=="simbody") {
//		PhysicsBody* p = new simbodyPhysics;
//	}
//}
/*
class SimbodyPhysics : public PhysicsBody {
public:
	SimbodyPhysics() {}
	SimTK::Body::Rigid simBody;
	SimTK::Body::Rigid getSimbody() { return simBody; }
};

class BulletPhysics : public PhysicsBody {
public:
int c;
};
*/

#endif

