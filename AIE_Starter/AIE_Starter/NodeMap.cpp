#include "NodeMap.h"
#include "raylib.h"
#include <iostream>
#include <vector>

namespace AIForGames {
	// global function for the AIForGames namespace. Calculates a series of Node Pointers that go from a start node to an end node.
	// This is a namespace global function so it is initialised in this .cpp file rather than being declared in the .h file
	std::vector<Node*> DijkstraSearch(Node* startNode, Node* endNode) {
		// Create a 'list' (here the list is a vector) of nodes/vertices not yet processed
		//std::vector<Node*>* openList = new std::vector<Node*>;
		std::vector<Node*> openList;
		// Create a 'list' (here the list is a vector) of nodes/vertices finished being processed
		//std::vector<Node*>* closedList = new std::vector<Node*>;
		std::vector<Node*> closedList;
		// A pointer to a Node that is the current node being processed
		Node* currentNode = new Node;

		std::cout << "Step 1: Check the starting and ending positions for existence." << std::endl;
		startNode == nullptr || endNode == nullptr												// If this is true
			? std::cout << "Error - start or end, or both, do not exist." << std::endl	// Do this (add functionality)
			: std::cout << "Start and end both exist. Continue." << std::endl;			// Else do this (add functionality)

		startNode == endNode																		// If this is true
			? std::cout << "Start and end are same - path is complete." << std::endl		// Do this (add functionality)
			: std::cout << "Start node and end node are different. Continue.\n" << std::endl;	// Else do this (add functionality)


		std::cout << "Step 2: Initialise the starting node." << std::endl;
		// Set distance from the starting node = 0.
		startNode->gScore = 0;
		std::cout << "Distance from starting node: 0." << std::endl;
		// Set no previous node for the origin.
		startNode->previousNode = nullptr;
		std::cout << "Origin has no previous node.\n" << std::endl;


		std::cout << "Step 3: Add the starting node to the list of open nodes\n." << std::endl;
		openList.push_back(startNode);
		//openList->push_back(startNode);


		std::cout << "Step 4: While the open list is not empty, run the Dijkstra search for the end node." << std::endl;
		while (openList.size() != 0) {
			// sort the open list so that the smallest g value (Dijkstra) or f value (A*) is at the front

			currentNode = *openList.begin();
			std::cout << "Step 4.1: First node in the open list has been set as the current node." << std::endl;

			std::cout << "Step 4.2: Check if the end node has been reached." << std::endl;
			if (currentNode == endNode) {
				std::cout << "Step 4.2: The end node has been reached - while loop ended.\n" << std::endl;
				break;
			}
			std::cout << "Step 4.2: The end node has not been reached - while loop continuing." << std::endl;

			// FIX - not sure how to convert from one vector to another type of vector...?
			openList.erase(*currentNode);

		}
	};

	// Default constructor
	NodeMap::NodeMap() {};

	// Destructor
	NodeMap::~NodeMap() {
		delete m_nodes;
		m_nodes = nullptr;
	};

	Node* NodeMap::GetNode(int x, int y) {
		// Return the node which is x nodes from the left and on the yth row
		return m_nodes[x + m_width * y];
	};


	void NodeMap::Draw() {
		// A Raylib color object for the ascii maze node objects (red)
		Color cellColour;
		cellColour.a = 255;
		cellColour.r = 255;
		cellColour.g = 0;
		cellColour.b = 0;

		// A Raylib color object for the ascii maze edge objects (grey)
		Color lineColour;
		lineColour.a = 64;
		lineColour.r = 128;
		lineColour.g = 128;
		lineColour.b = 128;

			// Loop over the cells in a nested loop for width and height
			// For each row element...
			for (int y = 0; y < m_height; y++) {
				// Look to each column element...
				for (int x = 0; x < m_width; x++) {
					// Create a temporary node which is a duplicate of the one whose coordinates are being examined
					Node* node = GetNode(x, y);
					// First, if the node is empty / doesn't exist...
					if (node == nullptr) {
						// draw a solid block in the empty space without a node
						DrawRectangle(
							(int)(x * m_cellSize),		// x
							(int)(y * m_cellSize),		// y
							(int)m_cellSize - 1,		// width (minus 1 pixel to separate blocks)
							(int)m_cellSize - 1,		// height (minus 1 pixel to separate blocks)
							cellColour);				// colour
					}

					// When there is a Node, we want to draw lines between it and its connections on its edges.
					else {
						// Draw the connections between the node and its neighbours, for every edge of this node
						for (int i = 0; i < node->connections.size(); i++) {
							// Create a temporary node pointer that points to each of the nodes connected to this one by an edge
							Node* other = node->connections[i].target;
							// Draw a line from the centre of this node to the centre of the other node (not their top-right {0,0} origins)
							DrawLine(
								// This is the standard text but I don't understand why I would use this when we define the position?
								//(x + 0.5f) * m_cellSize,
								//(y + 0.5f) * m_cellSize,
								(int)node->position.x,		// line start x
								(int)node->position.y,		// line start y
								(int)other->position.x,		// line end x
								(int)other->position.y,		// line end y
								lineColour);				// colour
						};
					};
				};
			};
	};

	void NodeMap::Initialise(std::vector<std::string> asciiMap, int cellSize) {
		// Set the map's cell size equal to the cell size passed in
		m_cellSize = cellSize;

		// Set the code for empty cells to nothing
		const char emptySquare = '0';

		// We will assume all strings are the same length, so we'll size the map according to the number of strings and the length of the first one
		// Height = total size
		m_height = asciiMap.size();
		// Width = size of first element
		m_width = asciiMap[0].size();

		// Dynamically allocate the size of the one-dimensional array of Node pointers equal to the dimensions of the map
		// "Make me a Node pointer which points to the starting memory position of n contiguous new Node pointers"
		m_nodes = new Node * [m_width * m_height];

		// loop over the strings, creating nodes for each string
		for (int y = 0; y < m_height; y++) {
			// Each row of the ascii map is an increment 'line', staring with the first element (index 0)
			std::string& line = asciiMap[y];
			// report to the user that you have a mis-matched string length if some row is of a different length than index 0
			if (line.size() != m_width) {
				std::cout << "Mismatched line #" << y << " in ASCII map (" << line.size() << " instead of " << m_width << ")" << std::endl;
			}

			for (int x = 0; x < m_width; x++) {
				// get the x-th character of the y-th row, or return an empty node if the string isn't long enough
				char tile = x			// if this (the target tile of this row)...
					< line.size()		// resolves this as true (has yet to be calculated)...
					? line[x]			// do this (return the x-th character of the y-th row)
					: emptySquare;		// else do this (leave the target tile empty if we're at the end of the y-th row)

				// create a node for anything other than a period character '.'
				m_nodes[x + m_width * y] = tile		// if this (the target tile)...
					== emptySquare					// resolves this as true (the target tile is empty)...
					? nullptr						// do this (don't create a node)
					: new Node(((float)x + 0.5f) * m_cellSize, ((float)y + 0.5f) * m_cellSize);		// else do this (create a node with x & y coordinates of where we have iterated up to in the ascii art map rows and columns, in teh middle of its 'cell' [hence the halving of cell size for height and width])
			}
		}

		/*
		"We’re using the length of the first string to calculate the width of the rectangular node map. We put in a debug warning if any of the strings are a different length but fail gracefully if they don’t match. Extra characters will never be read. Any missing characters on the end are assumed to be not navigable, so we won’t create a node for them.

		We now need to do a second pass across the grid of nodes and join them up with Edges. We can look at each Node and check its four neighbours, and create a pair of mutual Edges between them if we find an edge. For this exercise, we’ll assume that all edges are of equal cost of 1 to navigate.

		We only need to check each node to the west (x-1,y) and south (x, y-1) since the other two connections will be set up by the node in the next column or row.

		Loop over each of the nodes, creating connections between each node and its neightbout to the west and south on the grid. This will link up all nodes."
		*/

		// For each row element...
		for (int y = 0; y < m_height; y++) {
			// Look to each column element...
			for (int x = 0; x < m_width; x++) {
				// Create a temporary node which is a duplicate of the one whose coordinates are being examined
				Node* node = GetNode(x, y);
				// If the node exists...
				if (node) {
					// Check whether there is a node to the west, including a check for array over-runs if this is the west-most column
					Node* nodeWest = x			// if this...
						== 0					// resolves this as true...
						? nullptr				// do this
						: GetNode(x - 1, y);	// else do this

					// If it is true that there IS a node to the west...
					if (nodeWest) {
						// Connect this node to the western node and give it a distance of 1
						node->ConnectTo(nodeWest, 1);
						// Connect the western node to this node and give it a distance of 1
						nodeWest->ConnectTo(node, 1);
					};

					// Check whether there is a node to the south, including a check for array over-runs if this is the south-most row
					Node* nodeSouth = y			// if this...
						== 0					// resolves this as true...
						? nullptr				// do this
						: GetNode(x, y - 1);	// else do this

					// If it is true that there IS a node to the south...
					if (nodeSouth) {
						// Connect this node to the southern node and give it a distance of 1
						node->ConnectTo(nodeSouth, 1);
						// Connect the southern node to this node and give it a distance of 1
						nodeSouth->ConnectTo(node, 1);
					};
				};
			};
		};
	};
};