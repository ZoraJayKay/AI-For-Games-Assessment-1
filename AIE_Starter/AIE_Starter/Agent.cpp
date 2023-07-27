#include "Agent.h"

namespace AIForGames {
	Agent::Agent() {};

	Agent::Agent(NodeMap* _nodeMap, Behaviour* _behaviour) : m_current(_behaviour), m_nodeMap(_nodeMap), m_colour({ 255, 255, 0, 255 }) {};

	Agent::~Agent() {
		delete m_current;
	};

	std::vector<Node*> Agent::GetPath() {
		/*std::vector<Node*> path = m_pathAgent.GetPath();
		return path;*/
		return m_pathAgent.GetPath();
	}

	// This function updates the current Behaviour and then updates the PathAgent itself every frame
	void Agent::Update(float deltaTime) {
		if (m_current) {
			// Update the Behaviour of this Agent
			m_current->Update(this, deltaTime);
			// Update the PathAgent of this Agent
			m_pathAgent.Update(deltaTime);
		}
	};

	void Agent::Draw() {
		m_pathAgent.Draw();
	};

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

	void Agent::SetAgent(PathAgent agent) {
		m_pathAgent = agent;
	};

	// A function to return true if the PathAgent's path has been emptied (arrived at the node)
	bool Agent::PathComplete() {
		return m_pathAgent.GetPath().empty();
	};

	// A function to return the node map of this Agent's PathAgent
	NodeMap* Agent::GetMap() {
		return m_nodeMap;
	};
}
