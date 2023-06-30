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
#include "NodeMap.cpp"

using namespace std;
//using namespace AIForGames;

namespace AIForGames {
    int main(int argc, char* argv[])
    {
        // Initialization
        //--------------------------------------------------------------------------------------
        int screenWidth = 800;
        int screenHeight = 450;

        InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");

        SetTargetFPS(60);
        //--------------------------------------------------------------------------------------

        // ZORA: Tutorial requests making an ASCII art map to visualise the node map
        // Create a vector of strings, with each string representing a line of the map.
        // "We can use a code such as 0 = solid wall, 1 =  navigable space, and set up anything from a simple test shape to a complex maze for pathfinding to take place in"


        // 12x8 grid of chars denoting whether or not a cell is navigable (1) or impassable (0)
        vector<string> asciiMap;
        asciiMap.push_back("000000000000");     // row 1
        asciiMap.push_back("010111011100");     // row 2
        asciiMap.push_back("010101110110");     // row 3
        asciiMap.push_back("010100000000");     // row 4
        asciiMap.push_back("010111111110");     // row 5
        asciiMap.push_back("010000001000");     // row 6
        asciiMap.push_back("011111111110");     // row 7
        asciiMap.push_back("000000000000");     // row 8

        // Create a NodeMap class with a width, height and cell size, ie the spacing in pixels between consecutive squares in the grid. We’ll give it a function to initialize it’s data from the ASCII map declared above.

        NodeMap* map = new NodeMap();
        map->Initialise(asciiMap, 50);

        // Set the starting node for the Dijkstra search equal to the Node* in row 1, column 1 (in the ascii map)
        Node* start = map->GetNode(1, 1);
        Node* end = map->GetNode(10, 2);
        vector<Node*> nodeMapPath = map->DijkstraSearch(start, end);


        // Main game loop
        while (!WindowShouldClose())    // Detect window close button or ESC key
        {
            // Update
            //----------------------------------------------------------------------------------
            // TODO: Update your variables here
            //----------------------------------------------------------------------------------

            // Draw
            //----------------------------------------------------------------------------------
            BeginDrawing();

            map->Draw();

            ClearBackground(RAYWHITE);

            DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);

            EndDrawing();
            //----------------------------------------------------------------------------------
        }

        // De-Initialization
        //--------------------------------------------------------------------------------------   
        CloseWindow();        // Close window and OpenGL context
        //--------------------------------------------------------------------------------------

        return 0;
    }
}