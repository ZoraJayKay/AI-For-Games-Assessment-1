#include "NodeMap.h"
//#include "raylib.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>

using namespace std;

namespace AIForGames {
	

	// This is a global namespace function for the AIForGames namespace which will print the node path from back to front for a completed A* search.
	void NodeMap::Print(vector<Node*> path) {
		int counter = path.size();

		while (counter != 0) {
			for (Node* n : path) {
#ifndef NDEBUG
				cout << "Node [" << counter << "]: g score [" << n->gScore << "], h score [" << n->hScore << "], f score [" << n->fScore << "]." << endl;
#endif
				counter--;
			};
		};
	};

	// Default constructor
	NodeMap::NodeMap() {};

	// Destructor
	NodeMap::~NodeMap() {
		// Iterate through the dynamically-sized array and delete each element in turn, setting it to a nullptr
		for (int i = 0; i < (m_width * m_height); i++) {
			delete m_nodes[i];
			m_nodes[i] = nullptr;
		}

		// Delete the array object itself and set it to a nullptr
		delete[] m_nodes;
		m_nodes = nullptr;
	};

	void NodeMap::GetMapSize() {
		// Print out the memory allocation required for one map element
		std::cout << sizeof(m_nodes[0]) << std::endl;
	};

	// A function for returning the node which is closest to the mouse at the time of mouse button press
	Node* NodeMap::GetClosestNode(glm::vec2 worldPos) {
		int i = (int)(worldPos.x / m_cellSize);
		if (i < 0 || i > m_width) return nullptr;

		int j = (int)(worldPos.y / m_cellSize);
		if (j < 0 || j > m_height) return nullptr;

		return GetNode(i, j);
	}
	
	// A function to return the node which is x nodes from the left and on the yth row
	Node* NodeMap::GetNode(int x, int y) {
		return m_nodes[x + m_width * y];
	};

	// A function for drawing the best path calculated by a A* search
	void NodeMap::DrawPath(std::vector<Node*> path, Color colour) {
		// A Raylib color object for the shortest path through the ascii maze edge objects (blue)
		Color passedInColour = colour;

		// For every node with a non-null 'previous' pointer, draw a pathing line between itself and the previous node
		for (Node* node : path) {
			Node* other = node->previousNode;
			if (other != nullptr) {
				DrawLine(
					(int)other->position.x,
					(int)other->position.y,
					(int)node->position.x,
					(int)node->position.y,
					passedInColour);
			}			
		}

		// Debugging / informational printouts to the screen
#ifndef NDEBUG
		string noPath = "No path from start to end";
		string numberOfNodes = to_string(path.size());
		string numNodes = "Number of nodes in the path: " + numberOfNodes;

		if (path.size() == 0) {
			const char* nNodes = noPath.c_str();
			DrawText(nNodes, 50, AIForGames::sizeOfCell * 10.5, 15, WHITE);
		}

		else {
			const char* nNodes = numNodes.c_str();
			DrawText(nNodes, 50, AIForGames::sizeOfCell * 10.5, 15, WHITE);
		}	
#endif
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

						// Debug print the map coordinates of each empty cell (too visually busy with pathed cells included)
#ifndef NDEBUG
						string coordinateX = to_string(x);
						string coordinateY = to_string(y);
						string coordsYX = "(" + coordinateY + ", " + coordinateX + ")";
						const char* coords = coordsYX.c_str();
						DrawText(coords, (x * m_cellSize) + 2, (y * m_cellSize) + 2, 1, WHITE);
#endif
					}

					// When there is a Node, we want to draw lines between it and its connections on its edges.
					else {
						// Draw the connections between the node and its neighbours, for every edge of this node
						for (int i = 0; i < node->connections.size(); i++) {
							// Create a temporary node pointer that points to each of the nodes connected to this one by an edge
							Node* other = node->connections[i].targetNode;
							// Draw a line from the centre of this node to the centre of the other node (not their top-right {0,0} origins)
							DrawLine(
								(int)node->position.x,		// line start x
								(int)node->position.y,		// line start y
								(int)other->position.x,		// line end x
								(int)other->position.y,		// line end y
								lineColour);				// colour

#ifndef NDEBUG
							int cellGCost = node->gScore;
							string cellCost = to_string(cellGCost);
							string cellString = "g: (" + cellCost + ")";
							const char* gCost = cellString.c_str();
							
							int cellHCost = node->hScore;
							string stringHCost = to_string(cellHCost);
							string hCost = "h: (" + stringHCost + ")";
							const char* hCostChar = hCost.c_str();

							int cellFCost = node->fScore;
							string stringFCost = to_string(cellFCost);
							string fCost = "f: (" + stringFCost + ")";
							const char* fCostChar = fCost.c_str();

							Color clr;

							// If all values are nil (this node was not used for the path)
							if (node->gScore == 0 &&
								node->hScore == 0 &&
								node->fScore == 0) {
								// Then make its values grey
								clr = GRAY;
							}

							else {
								// Else let its colour be white (more visible)
								clr = WHITE;
							}

							DrawText(gCost, (x * m_cellSize) + 2, (y * m_cellSize) + 1, 0.5, clr);
							DrawText(hCostChar, (x * m_cellSize) + 2, (y * m_cellSize) + 16, 0.5, clr);
							DrawText(fCostChar, (x * m_cellSize) + 2, (y * m_cellSize) + 31, 0.5, clr);
							
#endif
						};
					};
				};
			};
	};

	void NodeMap::Initialise(std::vector<std::string> asciiMap, int cellSize) {
		// Set the map's cell size equal to the cell size passed in
		m_cellSize = cellSize;

		// Set the code for empty cells equal to nothing (0)
		const char emptySquare = '0';

		// We will assume all strings are the same length, so we'll size the map according to the number of strings and the length of the first one
		// Height = total size
		m_height = asciiMap.size();
		// Width = size of first element
		m_width = asciiMap[0].size();

		// Dynamically allocate the size of the one-dimensional array of Node pointers equal to the dimensions of the map
		// "Make me a Node pointer which points to the starting memory position of (width * height) contiguous new Node pointers"?
		// "Make me one new Node pointer which will point to an address that has enough contiguous memory to allocate the whole map (width * height)"?
		m_nodes = new Node * [m_width * m_height];

		// loop over the strings entered in AIE_Starter.cpp, creating nodes for each string character
		for (int y = 0; y < m_height; y++) {
			// Each row of the ascii map is an increment 'line', staring with the first element (index 0)
			std::string& line = asciiMap[y];
			// report to the user that you have a mis-matched string length if some row is of a different length than index 0
			if (line.size() != m_width) {
#ifndef NDEBUG
				std::cout << "Mismatched line #" << y << " in ASCII map (" << line.size() << " instead of " << m_width << ")" << std::endl;
				// Could add other proper error troubleshooting to add graceful failure but can't be bothered for the tutorial :)
#endif
			}

			for (int x = 0; x < m_width; x++) {
				// get the x-th character of the y-th row, or else return an empty node if the string isn't long enough
				char tile = x			// if this (the target tile of this row)...
					< line.size()		// resolves this as true (true that this cell has yet to be calculated)...
					? line[x]			// do this (return the x-th character of the y-th row)
					: emptySquare;		// else do this (leave the target tile empty if we're at the end of the y-th row)

				// create a node for anything other than a period character '.'
				m_nodes[x + m_width * y] = tile		// if this (the target tile)...
					== emptySquare					// resolves this as true (the target tile is empty [a '0'])...
					? nullptr						// do this (don't create a node)
					: new Node(((float)x + 0.5f) * m_cellSize, ((float)y + 0.5f) * m_cellSize);		// else do this (create a node with x & y coordinates of where we have iterated up to in the ascii art map rows and columns, in the middle of its 'cell' [hence the halving of cell size for height and width])
				std::cout << "Created node at position:\tColumn (" << x << ")\tRow (" << y << ")." << std::endl;
			}
		}

		/* From the tute:
		"We�re using the length of the first string to calculate the width of the rectangular node map. We put in a debug warning if any of the strings are a different length but fail gracefully if they don�t match. Extra characters will never be read. Any missing characters on the end are assumed to be not navigable, so we won�t create a node for them.

		We now need to do a second pass across the grid of nodes and join them up with Edges. We can look at each Node and check its four neighbours, and create a pair of mutual Edges between them if we find an edge. For this exercise, we�ll assume that all edges are of equal cost of 1 to navigate.

		We only need to check each node to the west (x-1,y) and south (x, y-1) since the other two connections will be set up by the node in the next column or row.

		Loop over each of the nodes, creating connections between each node and its neightbout to the west and south on the grid. This will link up all nodes."
		*/

		// For each row element...
		for (int y = 0; y < m_height; y++) {
			// Look to each column element...
			for (int x = 0; x < m_width; x++) {
				// Create a temporary node pointer which points to the address of the one whose coordinates are being examined
				Node* node = GetNode(x, y);
				// If the node exists...
				if (node) {
					// Create another temporary node pointer to check whether there is a node to the west, including a check for array over-runs if this is the west-most column
					Node* nodeWest = x			// if this...
						== 0					// resolves this as true ("the node to the west is empty")...
						? nullptr				// do this (make nodeWest a nullptr)
						: GetNode(x - 1, y);	// else do this (return the node to the west)

					// If it is true that there IS a node to the west (nodeWest is not a nullptr)...
					if (nodeWest) {
						// Connect this node to the western node and give it a distance of 1
						node->ConnectTo(nodeWest, 1);
						// Connect the western node to this node and give it a distance of 1
						nodeWest->ConnectTo(node, 1);
					};

					// Create another temporary node pointer to check whether there is a node to the south, including a check for array over-runs if this is the south-most row
					Node* nodeSouth = y			// if this...
						== 0					// resolves this as true ("the node to the south is empty")...
						? nullptr				// do this (make nodeSouth a nullptr)
						: GetNode(x, y - 1);	// else do this (return the node to the south)

					// If it is true that there IS a node to the south (nodeSouth is not a nullptr)...
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

	// A function to calculate the straight-line distance between one node and the end node
	int NodeMap::Heuristic(Node* a, Node* b) {
		// If the start or the end are null, return heuristic of nil.
		if (a == nullptr || b == nullptr) {
			return 0;
		}

		// otherwise calculate the 2D cartesian graph distance between the two points
		else {
			int xDist = (b->position.x - a->position.x);
			int yDist = (b->position.y - a->position.y);
			int heuristic = sqrt((xDist * xDist) + (yDist * yDist));

			// Calculate the heuristic as the straight-line distance divided by the average of both diagonal and straight-line cell size.
			heuristic /= AIForGames::sizeOfCell;

			return heuristic;
		}
	};

	// A function to set all pathfinding values to nil imbetween paths, so that only the nodes checked in the most recent path are shown on debugging
	void NodeMap::ClearMapVals() {
		for (int y = 0; y < this->m_height; y++) {
			for (int x = 0; x < this->m_width; x++) {
				if (GetNode(x, y) != nullptr) {
					GetNode(x, y)->gScore = 0;
					GetNode(x, y)->hScore = 0;
					GetNode(x, y)->fScore = 0;
				}				
			}
		}
	};

	// A function for randomly selecting a node from the node map
	Node* NodeMap::GetRandomNode() {
		// Start the random node as though it is returning an unnavigable node
		Node* node = nullptr;
		// Until the random node returns one that is navigable...
		while (node == nullptr) {
			// Get a random x from those available
			int x = rand() % m_width;
			// Get a random y from those available
			int y = rand() % m_height;
			node = GetNode(x, y);
		}

		// Return the navigable node that has been randomly selected
		return node;
	};

	// This is a function for calculating a series of Node Pointers that go from a start node to an end node.
	vector<Node*> NodeMap::AStarSearch(Node* startNode, Node* endNode) {
		// A lambda expression to be used as a function object for returning whether one node has a larger f score than another, inside a sort algorithm. I'm not searching by a property, always run the body of the expression based on the node's respective g scores.
		auto lambdaNodeSort = [](Node* const& lhs, Node* const& rhs) -> bool {
			// Return true if the left hand side integer is less than the right hand side integer, otherwise return false
			return lhs->fScore < rhs->fScore;
		};

		//	A* SEARCH FUNCTION -------------------------------------------------------------------------------
		//	1	----------------------------------------------------------------------------------------------------
#ifndef NDEBUG
		cout << "Step 1: Check the starting and ending node positions for existence on the map." << endl;
		startNode == nullptr || endNode == nullptr												// If this is true
			? cout << "Error - start or end, or both, do not exist." << endl	// Do this (add functionality)
			: cout << "Start and end both exist. Continue." << endl;			// Else do this (add functionality)

		startNode == endNode																		// If this is true
			? cout << "Start and end are same - path is complete." << endl		// Do this (add functionality)
			: cout << "Start node and end node are different. Continue.\n" << endl;	// Else do this (add functionality)
		// I know this doesn't actually do anything with errors - I would implement something if it was core to the tutorial, but not this time


		//	2	----------------------------------------------------------------------------------------------------
		cout << "Step 2: Initialise the starting node." << endl;
#endif

		// Set distance from the starting node = 0.
		startNode->gScore = 0;
#ifndef NDEBUG
		cout << "Distance from starting node to itself: " << startNode->gScore << "." << endl;
		// Set no previous node for the origin.
#endif
		startNode->previousNode = nullptr;
#ifndef NDEBUG
		cout << "Origin has no previous node.\n" << endl;


		//	3	----------------------------------------------------------------------------------------------------
		cout << "Step 3: Add the starting node to the list of open nodes.\n" << endl;
#endif
		// Create a collection (here the list is a vector) of nodes/vertices not yet processed
		vector<Node*> openList;

		// Create a collection (here the list is a vector) of nodes/vertices finished being processed
		vector<Node*> closedList;

		// A pointer to a Node that is the current node being processed
		Node* currentNode;

		currentNode = startNode;

		//openList.push_back(startNode);
		openList.push_back(currentNode);


		//	4	----------------------------------------------------------------------------------------------------
		// debug counter
		int counter = 0;
#ifndef NDEBUG
		std::cout << "Step 4: While the open list is not empty, run the A* search for the end node.\nBegin while loop\t----------" << endl;
#endif
		while (openList.size() != 0) {
			//	4.1	----------------------------------------------------------------------------------------------------
			/* Sort the open list so that the smallest g value (Dijkstra) or f value (A*) is at the front.
			I'm using the "Compare" container template version of the sort() function from the <algorithm> header file, together with a lambda expression which accepts two nodes and sorts them according to a comparison between their f scores, returning true/false based on the comparison. */
			sort(
				openList.begin(),
				openList.end(),
				lambdaNodeSort);
#ifndef NDEBUG
			cout << "Step 4.1: The open list has been sorted by ascending node f score." << endl;
			cout << "Step 4.1.1: Begin processing node " << counter << "." << endl;
#endif
			counter++;

			//	4.2	----------------------------------------------------------------------------------------------------
			currentNode = *openList.begin();
#ifndef NDEBUG
			cout << "Step 4.2: First node in the open list (f score of " << currentNode->gScore << ") has been set as the current node." << endl;

			//	4.3	----------------------------------------------------------------------------------------------------
			cout << "Step 4.3: Check if the end node has been reached." << endl;
#endif
			if (currentNode == endNode) {
#ifndef NDEBUG
				std::cout << "Step 4.3a: The end node has been reached - while loop will end.\n" << endl;
#endif
				break;
			}
#ifndef NDEBUG
			cout << "Step 4.3b: The end node has not been reached - while loop will continue." << endl;
#endif

			//	4.4	----------------------------------------------------------------------------------------------------
			// 4.4.1: Create an iterator to find the location of the current node in the open list
			vector<Node*>::iterator itr_00 = find(openList.begin(), openList.end(), currentNode);
			// Save the position in the list where the current node was found
			int index_00 = 0;
			index_00 = distance(openList.begin(), itr_00);
			// Erase the found node from the list
			openList.erase(openList.begin() + index_00);
#ifndef NDEBUG
			cout << "Step 4.4: The current node has been removed from the open list." << endl;
#endif

			//	4.5	----------------------------------------------------------------------------------------------------
			closedList.push_back(currentNode);
#ifndef NDEBUG
			cout << "Step 4.5: The current node has been added to the closed list (it has finished being processed)." << endl;

			//	4.6	----------------------------------------------------------------------------------------------------
			cout << "Step 4.6: Determine whether the target Nodes of the current Node's Edges have already been processed or not." << endl;
#endif
			// For all edges of the currentNode...
			for (Edge targetEdge : currentNode->connections) {
				// 4.6.1: Create iterators to look at the target node of the edge and see whether it is in the closed list or open list
				vector<Node*>::iterator itr_01 = find(closedList.begin(), closedList.end(), targetEdge.targetNode);
				vector<Node*>::iterator itr_02 = find(openList.begin(), openList.end(), targetEdge.targetNode);
#ifndef NDEBUG
				cout << "Step 4.6.1: An Edge was found and its target Node has been searched for in the closed and open lists." << endl;
#endif

				// Save the position in the closed and open lists where this edge's target node was found
				int index_01 = 0;
				index_01 = distance(closedList.begin(), itr_01);
				int index_02 = 0;
				index_02 = distance(openList.begin(), itr_02);

				// 4.6.2: If the iterator did not find the target node in the closed list (if it reached the end of the closed list) Then the target node of this edge needs to be processed.
				if (itr_01 == closedList.end()) {
#ifndef NDEBUG
					cout << "Step 4.6.2: This Edge was not found in the closed list (its processing has started but not yet finished)." << endl;
#endif
					// 4.6.2.1a: Calculate a hypothetical g score (for comparison against the pre-existing g score)
					int calcdG = currentNode->gScore + targetEdge.cost; 

					// 4.6.2.1b: Calculate a heuristic score between this node and the end
					int calcdH = Heuristic(targetEdge.targetNode, endNode);

					// 4.6.2.1c: Calculate a total score
					int calcdF = calcdG + calcdH;

#ifndef NDEBUG
					cout << "Step 4.6.2.1a-c: This Edge has a target Node with a calculated g score of [" << calcdG << "], a calculated h score of [" << calcdH << "], and a calculated f score of [" << calcdF << "]." << endl;
#endif
					// 4.6.2.2a: Then, if this node is not already in the open list...
					if (itr_02 == openList.end()) {
#ifndef NDEBUG
						cout << "Step 4.6.2.2a: The target Node of this Edge was not found in the open list (its processing has not yet started)." << endl;
#endif
						// Make the g score of the target node equal to the g score of the current node plus the cost of this edge
						targetEdge.targetNode->gScore = calcdG;

						targetEdge.targetNode->hScore = calcdH;

						// Make the f score of the target node equal to g + h
						targetEdge.targetNode->fScore = calcdF;

#ifndef NDEBUG
						cout
							<< "Step 4.6.2.2a(i): The target Node of this Edge has had its g score set to ["
							<< targetEdge.targetNode->gScore
							<< "], its h score set to [" << targetEdge.targetNode->hScore 
							<< "], and its f score set to [" << targetEdge.targetNode->fScore << "]." << endl;
#endif
						// Make the current node be the 'previous' node of the target
						targetEdge.targetNode->previousNode = currentNode;
#ifndef NDEBUG
						cout << "Step 4.6.2.2a(ii): The current Node is now the parent of the target Node on this Edge." << endl;
#endif
						// Add to the open list for processing
						openList.push_back(targetEdge.targetNode);
#ifndef NDEBUG
						cout << "Step 4.6.2.2a(iii): The target Node of this Edge has been added to the open list (its processing has started).\n" << endl;
#endif
					}

					// 4.6.2.2b: Otherwise if this node is already in the openList AND if its calculated f score is lower than its existing f score...
					else if (calcdF < targetEdge.targetNode->fScore) {
#ifndef NDEBUG
						cout << "Step 4.6.2.2b: This edge was found in the open list (its processing has started but not yet finished) and its f score through this Edge is lower than its existing f score through some other path (this path is shorter)." << endl;
#endif
						// Make the g score of the target node equal to the g score of the current node plus the cost of this edge
						targetEdge.targetNode->gScore = calcdG;

						targetEdge.targetNode->hScore = calcdH;

						// Make the f score of the target node equal to g + h
						targetEdge.targetNode->fScore = calcdF;
#ifndef NDEBUG
						cout << "Step 4.6.2.2b(i): The target Node of this Edge has had its g score set to [" << targetEdge.targetNode->gScore
							<< "] and its f score set to [" << targetEdge.targetNode->fScore << "]." << endl;
#endif
						targetEdge.targetNode->previousNode = currentNode;
#ifndef NDEBUG
						cout << "Step 4.6.2.2b(ii): The current Node is now the parent of the target Node on this Edge.\n" << endl;
#endif
					}
				}
			};
		};

		
#ifndef NDEBUG
		std::cout << "End while loop\t--------\n" << endl;

		//	5	----------------------------------------------------------------------------------------------------
		cout << "Step 5: Create a path in reverse from the end node to the start node." << endl;
#endif
		vector<Node*> path;

		currentNode = endNode;
		while (currentNode != nullptr) {
			path.insert(path.begin(), currentNode);
			currentNode = currentNode->previousNode;
		}

		/*if (currentNode->previousNode != nullptr) {
			currentNode->previousNode = nullptr;
		}	*/	

#ifndef NDEBUG
		cout << "A vector of Nodes (the 'path') has been created. Size: [" << path.size() << "] nodes." << endl;
#endif

		if (*path.begin() != startNode) {
			path.clear();
		}

		//// If there's only one node in the path then no path is possible; make the path null before returning it.
		//if (path.size() < 2) {
		//	path.clear();
		//	std::cout << "TRIGGERED THE THING" << std::endl;
		//}

		return path;
	};
};