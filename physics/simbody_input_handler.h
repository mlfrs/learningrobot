#ifndef _SIMBODY_INPUT_HANDLER_H_
#define _SIMBODY_INPUT_HANDLER_H_

#include "Simbody.h"

class SimbodyInputHandler : public SimTK::PeriodicEventHandler {
public:
	SimbodyInputHandler(SimTK::Visualizer::InputSilo& silo, 
//	const SimTK::Motion::Steady& motor, 
	SimTK::Real interval); 

void handleEvent(SimTK::State& state, SimTK::Real accuracy,
	bool& shouldTerminate) const override;
private:
	SimTK::Visualizer::InputSilo& m_silo;
//	SimTK::Motion::Steady         m_motor;
};

#endif
