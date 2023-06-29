#pragma once
#include "Pathfinding.h"
#include <string>
#include <algorithm>

// Use the same namespace as the one set up by the tutorial
namespace AIForGames {
	// Create a new class within the namespace to hold the map of nodes
	class NodeMap
	{
		// Map variables
		int m_width;
		int m_height;

		// The size of every separate 'cell' within the map
		float m_cellSize;

		// From the tute: "The Node** variable nodes is essentially a dynamically allocated one dimensional array of Node pointers."
		Node** m_nodes;

	public:
		// Default constructor
		NodeMap();

		// Destructor
		~NodeMap();

		// A function for the purposes of setting up a node map according to a vector of strings, called 'asciiMap', and a given size for each node to be
		// From the tute: "In the Initialise function we will allocate this array to match the width and height of the map (determined by the vector of strings passed in) and fill it with either newly allocated Nodes or null pointers for each square on the grid."
		void Initialise(std::vector<std::string> asciiMap, int cellSize);

		// A function to return the Node* for a given pair of coordinates
		Node* GetNode(int x, int y);

		// A function to draw the map to the screen
		void Draw();
	};
}