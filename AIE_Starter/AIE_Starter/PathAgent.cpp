#include "PathAgent.h"
#include "NodeMap.h"
#include <cmath>
#include "raylib.h"

namespace AIForGames {
	PathAgent::PathAgent() {};
	PathAgent::~PathAgent() {};

	std::vector<Node*> PathAgent::GetPath() {
		return m_path;
	}

	void PathAgent::SetNode(Node* node) {
		m_currentNode = node;
		m_position.x = node->position.x;
		m_position.y = node->position.y;
	};

	void PathAgent::SetSpeed(int spd) {
		m_speed = spd;
	};

	void PathAgent::Update(float deltaTime) {
		// 1: If the path is empty, end the function
		if (m_path.size() == 0) {
			return;
		}

		// 2: Otherwise, calculate the distance to the next node and the unit vector to that node
		// 2.a: DISTANCE TO NEXT NODE
		// Distance to the next node = (position of next node - current position)
		// Distance to the next node = magnitude of the vector between the two nodes (they are both points)
		// 2.a.i: Calculate the vector between the destination node and current node
		int xDistance = m_path[m_currentIndex + 1]->position.x - m_path[m_currentIndex]->position.x;
		int yDistance = m_path[m_currentIndex + 1]->position.y - m_path[m_currentIndex]->position.y;

		glm::vec2 directionVector = {xDistance, yDistance};

		// 2.a.ii: Calculate the distance (the vector's magnitude [square root of its coordinates squared])
		int distance = sqrt(
			(directionVector.x * directionVector.x) +
			(directionVector.y * directionVector.y));

		// 2.b: UNIT VECTOR TO NEXT NODE
		// 2.b.i: Calculate a unit vector by dividing the vector with its own magnitude
		glm::vec2 unitVector;
		unitVector.x = directionVector.x / distance;
		unitVector.y = directionVector.y / distance;
		
		// 2.b.ii: Subtract speed * deltaTime from the distance (how much we're going to move this frame)
		int frameMoveTick = distance - (m_speed * deltaTime);

		// 2.b.iii: If distance is greater than zero, then this frame we're just moving towards the target node; add speed * deltaTime * unit vector to our position
		if (distance > 0) {
			m_path[m_currentIndex]->position.x += (m_speed * deltaTime * unitVector.x);
			m_path[m_currentIndex]->position.y += (m_speed * deltaTime * unitVector.y);
		}
		
		// 3: Otherwise, we've overshot the node.
		// 3.a.i: Add one to currentIndex.
		m_currentIndex += 1;

		// 3.a.ii: If we've reached the end of our path...
		if (m_path[m_currentIndex] == *m_path.end()) {
			// Snap to the final node...
			m_path[m_currentIndex] = *m_path.end();
			// ... and empty the path so future updates do nothing.
			m_path.clear();
		};

		// 3.a.iii: If we have a next node...
		if (m_path[m_currentIndex] != *m_path.end()) {
			// Then distance with the subtracted speed * deltaTime tells us how far we've overshot the node if we invert it.
			int overshoot = -(distance + (m_speed * deltaTime));

			// Find the unit vector from our previous node to the new next node...
			xDistance = m_path[m_currentIndex + 1]->position.x - m_path[m_currentIndex - 1]->position.x;
			yDistance = m_path[m_currentIndex + 1]->position.y - m_path[m_currentIndex - 1]->position.y;

			// Refresh vector
			directionVector = {xDistance, yDistance};

			// Refresh magnitude / distance
			distance = sqrt(
				(directionVector.x * directionVector.x) +
				(directionVector.y * directionVector.y));

			// Refresh unit vector 
			unitVector.x = directionVector.x / distance;
			unitVector.y = directionVector.y / distance;

			// ... and move along this vector by the overshoot distance from the previous next node.
			m_path[m_currentIndex]->position.x += overshoot * unitVector.x;
			m_path[m_currentIndex]->position.y += overshoot * unitVector.y;
		};		
	};

	void PathAgent::GoToNode(Node* node) {
		// Call the pathfinding function to make and store a path from the current node to the given destination
		m_path = NodeMap::DijkstraSearch(m_currentNode, node);
		// When we recalculate the path our next node is always the first one along the path, so we reset currentIndex to 0.
		m_currentIndex = 0;
	};

	void PathAgent::Draw() {
		Color agentColour;

		agentColour.a = 255;
		agentColour.r = 255;
		agentColour.g = 0;
		agentColour.b = 255;
		
		DrawCircle((int)m_position.x, (int)m_position.y, 8, agentColour);
	};
}