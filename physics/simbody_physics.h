/*
 * Authors: Cormac Guerin
 *
 * Name: simbody_physics.h
 *
 * Create a simbody physics Body::Rigid
 *
 * This would typically be stored in an object
 * along with say the graphics translations etc
 *
 */

#ifndef _SIMBODY_PHYSICS_H_
#define _SIMBODY_PHYSICS_H_

#include "Simbody.h"
#include "physics.h"

using namespace SimTK;

class SimbodyPhysics: public Physics {
	public:
		SimbodyPhysics() :
		Body::Rigid getPhyNode()
		{ 
			Body::Rigid bodyInfo(MassProperties(1.0, Vec3(0), UnitInertia(1)));
			return bodyInfo; 
		}
};

#endif

