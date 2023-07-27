#pragma once
#include "NodeMap.h"
#include "PathAgent.h"
#include "raylib.h"
#include "Behaviour.h"

#define RAYGUI_IMPLEMENTATION
#define RAYGUI_SUPPORT_ICONS

namespace AIForGames {
	class NodeMap;

	class Agent
	{
	public:
		Agent();
		Agent(NodeMap* _nodeMap, Behaviour* _behaviour);
		~Agent();

		void Update(float deltaTime);
		void Draw();
		void GoTo(glm::vec2 point);
		void SetNode(Node* node);
		void SetAgent(PathAgent agent);
		std::vector<Node*> GetPath();
		bool PathComplete();
		NodeMap* GetMap();

	private:
		PathAgent m_pathAgent;
		Behaviour* m_current;
		NodeMap* m_nodeMap;
		Color m_colour;
	};
}