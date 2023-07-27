#pragma once
#include "Behaviour.h"

namespace AIForGames {
	
	// A derived Behaviour class to activate on mouse click
	class GoToPointBehaviour : public Behaviour
	{
		// This is a virtual void Update to override the pure virtual void Update in the base abstract class.
		virtual void Update(Agent* agent, float deltaTime);
	};
}