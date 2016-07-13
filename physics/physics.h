
#ifndef _PHYSICS_H_
#define _PHYSICS_H_

#include "mlfrs_simbody.h"

class physics 
{
private:
	MlfrsSimbody simbody;

public:
	physics();
	~physics();
	
	void init();

};

#endif

