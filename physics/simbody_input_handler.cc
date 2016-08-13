#include "simbody_input_handler.h"

using namespace SimTK;

SimbodyInputHandler::SimbodyInputHandler(Visualizer::InputSilo& silo,
//								  const Motion::Steady&  motor,
								  Real interval)
: PeriodicEventHandler(interval), m_silo(silo) {}

void SimbodyInputHandler::handleEvent(State& state, Real accuracy,
								   bool& shouldTerminate) const {

	while (m_silo.isAnyUserInput()) {
		unsigned key, modifiers;
		while (m_silo.takeKeyHit(key,modifiers))
			if (key == Visualizer::InputListener::KeyEsc) {
				shouldTerminate = true;
				m_silo.clear();
				return;
			}

//		int whichSlider; Real sliderValue;
//		while (m_silo.takeSliderMove(whichSlider, sliderValue))
//			if (whichSlider == SpeedControlSlider)
//				m_motor.setRate(state, sliderValue);

	}
}

