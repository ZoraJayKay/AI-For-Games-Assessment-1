#include "Agent.h"

namespace AIForGames {
	Agent::Agent() {};

	Agent::Agent(NodeMap* _nodeMap, Behaviour* _behaviour) : m_current(_behaviour), m_nodeMap(_nodeMap), m_colour({ 255, 255, 0, 255 }) {};

	Agent::~Agent() {
		delete m_current;
	};	

	// This function updates the current Behaviour and then updates the PathAgent itself every frame
	void Agent::Update(float deltaTime) {
		if (m_current) {
			// Update the Behaviour of this Agent
			m_current->Update(this, deltaTime);
			// Update the PathAgent of this Agent
			m_pathAgent.Update(deltaTime);
		}
	};

	void Agent::Draw() {};

	// A function that finds the nearest node to the given point and calculates a path to it.
	void Agent::GoTo(glm::vec2 point) {
		Node* end = m_nodeMap->GetClosestNode(point);
		m_nodeMap->ClearMapVals();
		// On mouse click, set the agent's current node = the nearest node to its current world position
		m_pathAgent.SetAgentCurrentNode(m_nodeMap->GetClosestNode(glm::vec2(m_pathAgent.GetAgentPosition().x, m_pathAgent.GetAgentPosition().y)));
		m_pathAgent.GoToNode(end);
	};

	// A function for setting the physical location of the PathAgent inside this Agent
	void Agent::SetNode(Node* node) {
		// Assign the parameter node passed in as the current node of the PathAgent inside this Agent
		m_pathAgent.SetAgentCurrentNode(node);
		// Assign the Cartesian coordinates of the node passed in as the coordinates of the PathAgent inside this Agent
		m_pathAgent.SetAgentPosition(glm::vec2(node->position.x, node->position.y));
	}
}
