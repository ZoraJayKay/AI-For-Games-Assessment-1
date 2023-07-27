/*******************************************************************************************
*
*   raylib [core] example - Basic window
*
*   Welcome to raylib!
*
*   To test examples, just press F6 and execute raylib_compile_execute script
*   Note that compiled executable is placed in the same folder as .c file
*
*   You can find all basic examples on C:\raylib\raylib\examples folder or
*   raylib official webpage: www.raylib.com
*
*   Enjoy using raylib. :)
*
*   This example has been created using raylib 1.0 (www.raylib.com)
*   raylib is licensed under an unmodified zlib/libpng license (View raylib.h for details)
*
*   Copyright (c) 2014 Ramon Santamaria (@raysan5)
*
********************************************************************************************/


/*
AIE Pathfinding tutorial #1
In this tutorial series we’ll look at creating an implementation of Djikstra’s Shortest Path algorithm.

There are three tutorials:

Creating a Node Graph
Calculating a Path
Creating a Pathing Agent

*/


#include "raylib.h"

#define RAYGUI_IMPLEMENTATION
#define RAYGUI_SUPPORT_ICONS
#include "raygui.h"
#include "Pathfinding.h"
#include <string>;
//#include "memory.h"
#include "NodeMap.h"
#include <iostream>
#include "Agent.h"
#include "PathAgent.h"
#include "GoToPointBehaviour.h"
#include "WanderBehaviour.h"

using namespace std;
using namespace AIForGames;

int main(int argc, char* argv[])
{
	// Initialization
	//--------------------------------------------------------------------------------------
	int screenWidth = 1200;
	int screenHeight = 700;

	InitWindow(screenWidth, screenHeight, "Zora Jane Kerr: Artificial Intelligence for Games (Assessment 1 - Implement the A* Pathfinding Algorithm) AIE, 2023 (student year 1)");

	SetTargetFPS(60);
	//--------------------------------------------------------------------------------------

	// ZORA: Tutorial requests making an ASCII art map to visualise the node map
	// Create a vector of strings, with each string representing a row line of the map.
	// "We can use a code such as 0 = solid wall, 1 =  navigable space, and set up anything from a simple test shape to a complex maze for pathfinding to take place in"
	
	vector<string> asciiMap;

	// 20x10 grid of chars denoting whether or not a cell is navigable (1) or impassable (0) /// ALTERNATE MAP
	asciiMap.push_back("00000000000000000000");     // row 1
	asciiMap.push_back("01011101110000111000");     // row 2
	asciiMap.push_back("01010111011111101100");     // row 3
	asciiMap.push_back("01010000000010100110");     // row 4
	asciiMap.push_back("01011111111011110010");     // row 5
	asciiMap.push_back("01000000100010011110");     // row 6
	asciiMap.push_back("01111100001110010010");     // row 7
	asciiMap.push_back("00000111111000010000");     // row 8
	asciiMap.push_back("01110101001111111110");     // row 9
	asciiMap.push_back("00000000000000000000");     // row 10

	// Create a NodeMap class with a width, height and cell size, ie the spacing in pixels between consecutive squares in the grid. We’ll give it a function to initialize its data from the ASCII map declared above.
	NodeMap* map = new NodeMap();
	
	map->Initialise(asciiMap, AIForGames::sizeOfCell);

	// Set the starting node for the A* search equal to the Node* in column index 1, row index 1 (in the ascii map)
	Node* start = map->GetNode(3, 2);

	// Create a new player agent behaviour with the existing node map and the 'point and click' behaviour
	Agent player_behaviour(map, new GoToPointBehaviour());
	PathAgent playerAgent;
	playerAgent.SetSpeed(64);
	player_behaviour.SetAgent(playerAgent);
	player_behaviour.SetNode(start);
	

	Agent agent_behaviour_01(map, new WanderBehaviour());
	PathAgent wanderingAgent;
	wanderingAgent.SetSpeed(32);
	agent_behaviour_01.SetAgent(wanderingAgent);
	agent_behaviour_01.SetNode(map->GetRandomNode());
	agent_behaviour_01.SetNode(map->GetNode(1,8));


	// Time at commencement of pathfinding
	float time = (float)GetTime();
	float deltaTime;

	// Main game loop
	while (!WindowShouldClose())    // Detect window close button or ESC key
	{
		// Time at moment of update
		float fTime = (float)GetTime();

		// Change in time since last update
		deltaTime = fTime - time;

		// Reset timer for this update
		time = fTime;

		// Draw
		//----------------------------------------------------------------------------------
		BeginDrawing();

		ClearBackground(BLACK);

		// Draw the nodes of the map
		map->Draw();
		// Draw a line that shows the current path of the PathAgent inside the Agent that is passed in
		map->DrawPath(player_behaviour.GetPath(), BLUE);
		map->DrawPath(agent_behaviour_01.GetPath(), GREEN);
		// Update the behaviour of the Agent that encapsulates the PathAgent
		player_behaviour.Update(deltaTime);
		agent_behaviour_01.Update(deltaTime);
		// Draw the path of the PathAgent inside the Agent
		player_behaviour.Draw();
		agent_behaviour_01.Draw();
		// Finish
		EndDrawing();

		//----------------------------------------------------------------------------------


		//if (IsMouseButtonPressed(0)) {
		//	Vector2 mousePos = GetMousePosition();
		//	Node* end = map->GetClosestNode(glm::vec2(mousePos.x, mousePos.y));
		//	map->ClearMapVals();
		//	// On mouse click, set the agent's current node = the nearest node to its current world position
		//	agent.SetAgentCurrentNode(map->GetClosestNode(glm::vec2(agent.GetAgentPosition().x, agent.GetAgentPosition().y)));
		//	agent.GoToNode(end);
		//}


		//// ----- This code is just for demonstrating moving the path's origin -----
		//// If left mouse is clicked...
		//if (IsMouseButtonPressed(0)) {
		//	map->ClearMapVals();
		//	Vector2 mousePos = GetMousePosition();
		//	start = map->GetClosestNode(glm::vec2(mousePos.x, mousePos.y));
		//	nodeMapPath = map->AStarSearch(start, end);
		//	nodeMapPath = NodeMap::AStarSearch(start, end);
		//}

		//// ----- This code is just for demonstrating moving the path's destination -----
		////// If right mouse is clicked...
		//if (IsMouseButtonPressed(1)) {
		//	map->ClearMapVals();
		//	Vector2 mousePos = GetMousePosition();
		//	end = map->GetClosestNode(glm::vec2(mousePos.x, mousePos.y));
		//	nodeMapPath = map->AStarSearch(start, end);
		//	nodeMapPath = NodeMap::AStarSearch(start, end);
		//}

		
		//map->DrawPath(nodeMapPath);
		//map->DrawPath(agent.GetPath());

		/*agent.Update(deltaTime);
		agent.Draw();*/

		

		//EndDrawing();

		//----------------------------------------------------------------------------------
	}

	// De-Initialization
	//--------------------------------------------------------------------------------------   
	CloseWindow();        // Close window and OpenGL context
	//--------------------------------------------------------------------------------------

	delete map;
	map = nullptr;	

	return 0;
}