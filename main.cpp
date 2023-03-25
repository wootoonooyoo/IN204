#include <iostream>
#include <array>
#include <random>
#include "grid.cpp"
#include "piece.hpp"
#include "window.cpp"


void moveCheckRender(gameGrid stagingGrid, gameWindow w, piece *piece1, std::pair<int,int> boardSize, int x_change=0, int y_change=0, bool isRotation = false)
{
    // Move
    piece1->move(x_change,y_change);

    // Check
    if(!stagingGrid.checkMoveValidity(*piece1))
    {
        if(isRotation)
        {
            piece1->revertRotation();
        }
        else
        {
            piece1->revert();
        }
        return;
    }

    // Render
    stagingGrid.updateWithPiece(*piece1);
    w.renderGrid(stagingGrid.array(),boardSize); // base layer which contains the wall and pieces
    w.drawGrid(stagingGrid.size());              // grid for aesthetic reasons
    w.render();                                  // render the window    

    // Prepare next iteration
    stagingGrid.resetGrid();
    w.clearStagingBuffer();
}

bool moveCheckLockRender(gameGrid stagingGrid, gameGrid *baseGrid,gameWindow w, piece *piece1, std::pair<int,int> boardSize, int x_change=0, int y_change=0)
{
    // Move
    piece1->move(x_change,y_change);

    // Check
    if(!stagingGrid.checkMoveValidity(*piece1))
    {
        // Lock
        return true;
    }

    // Render
    stagingGrid.updateWithPiece(*piece1);
    w.renderGrid(stagingGrid.array(),boardSize); // base layer which contains the wall and pieces
    w.drawGrid(stagingGrid.size());              // grid for aesthetic reasons
    w.render();                                  // render the window    

    // Prepare next iteration
    stagingGrid.resetGrid();
    w.clearStagingBuffer();

    return false;
}


int rngGenerator(int min=0, int max=6)
{
    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<int> uni(min,max);
    return uni(rng);
}

int main()
{
    
    while (true)
    {
        // Generate
        SDL_Event event;
        gameGrid stagingGrid(10,20,3);
        gameGrid baseGrid(10,20,3);
        piece piece1(3,3,1,0);
        gameWindow w(100, 100, 900, 900);
        std::pair<int,int> coords{6,1};

        int gravityInterval = 600; // ms
        int tickBaseline = 0;      // ms
        int gravityCount = 0;
        bool lock = false;

        int shapeNumber = rngGenerator(0,6);
        int orientationNumber = rngGenerator(0,3);
        
        moveCheckRender(stagingGrid, w, &piece1, stagingGrid.size());
        tickBaseline = SDL_GetTicks64() - gravityInterval;

        for(int i=0; i<100; i++)
        {
            
            while (!lock)
            {    
                // Quit the programme
                if (SDL_PollEvent(&event) && event.type == SDL_QUIT)
                {   
                    exit(0);
                }

                // Check gravity
                if (SDL_GetTicks64() - tickBaseline >= gravityInterval)
                {
                    tickBaseline = SDL_GetTicks64();
                    stagingGrid = baseGrid;
                    lock = moveCheckLockRender(stagingGrid, &baseGrid, w, &piece1, stagingGrid.size(), 0, 1);
                }
                
                // Check keyboard input
                if (event.type == SDL_KEYDOWN)
                {
                    switch (event.key.keysym.sym)
                    {
                        case SDLK_LEFT:
                            stagingGrid = baseGrid;
                            moveCheckRender(stagingGrid, w, &piece1, stagingGrid.size(), -1, 0);
                            break;
                        case SDLK_RIGHT:      
                            stagingGrid = baseGrid;                  
                            moveCheckRender(stagingGrid, w, &piece1, stagingGrid.size(), 1, 0);
                            break;
                        case SDLK_DOWN:
                            stagingGrid = baseGrid;
                            moveCheckRender(stagingGrid, w, &piece1, stagingGrid.size(), 0, 1);
                            break;
                        case SDLK_UP:
                            piece1.rotate();
                            stagingGrid = baseGrid;
                            moveCheckRender(stagingGrid, w, &piece1, stagingGrid.size(), 0, 0, true);
                            break;
                        default:
                            break;
                    }            

                }
            } 
        
            // save into baseGrid
            piece1.revert();
            baseGrid.updateWithPiece(piece1);
            lock = false;

            // check if there are any lines to clear
            baseGrid.clearLines();
            baseGrid.moveAllLinesDown();

            // generate next Shape
            shapeNumber = rngGenerator(0,6);
            orientationNumber = rngGenerator(0,3);
            piece1 = piece(shapeNumber,orientationNumber,3,0); 

        }

        exit(0);

    }
}

    
    