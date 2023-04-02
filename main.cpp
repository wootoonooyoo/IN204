#include <iostream>
#include <array>
#include <random>
#include "grid.cpp"
#include "piece.hpp"
#include "window.cpp"
#include <chrono>

bool moveCheckRender(gameGrid stagingGrid, gameGrid *baseGrid, gameWindow w, piece *piece1, std::pair<int, int> boardSize, queue q, int x_change = 0, int y_change = 0, bool isRotation = false, bool isGravityTriggered = false)
{
    // Move
    piece1->move(x_change, y_change);

    // Check
    if(isGravityTriggered && !stagingGrid.checkMoveValidity(*piece1))
    {
        // Lock
        return true;
    }
    else if (!isGravityTriggered && !stagingGrid.checkMoveValidity(*piece1))
    {
        if (isRotation)
        {
            piece1->revertRotation();
        }
        else
        {
            piece1->revert();
        }
        return false;
    }

    // Render
    stagingGrid.updateWithPiece(*piece1);
    w.renderGrid(stagingGrid.array(), boardSize); // base layer which contains the wall and pieces
    w.drawGrid(stagingGrid.size());               // grid for aesthetic reasons
    w.renderQueue(q);                               // queue
    w.render();                                   // render the window
    
    // Prepare next iteration
    stagingGrid.resetGrid();
    w.clearStagingBuffer();

    return false;
}

int main()
{
    int score_to_add = 0;
    // TTF_Init();
    auto time_last_lvl = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::now();
    while (true)
    {
        // Generate
        SDL_Event event;
        gameGrid stagingGrid(10, 20, 4);
        gameGrid baseGrid(10, 20, 4);
        piece piece1(3, 3);
        gameWindow w(100, 100, 900, 900);
        std::pair<int, int> coords{6, 0};
        int level = 1;
        int gravityInterval = 400 + 200*level; // ms
        int tickBaseline = 0;                    // ms
        int gravityCount = 0;
        bool lock = false;
        bool play = true;

        // generate a queue
        queue q(3);
        q.generateMax();
        std::pair<int, int> pieceInformation = q.popAndGenerate();

        // Render first piece
        int shapeNumber = pieceInformation.first;
        int orientationNumber = pieceInformation.second;
        piece1 = piece(shapeNumber, orientationNumber);
        moveCheckRender(stagingGrid, &baseGrid, w, &piece1, stagingGrid.size(), q);
        
        // Gravity
        tickBaseline = SDL_GetTicks64() - gravityInterval;

        for (int i = 0; i < 100; i++)
        {
            while (!lock)
            {
                // Quit the programme
                if (SDL_PollEvent(&event) && event.type == SDL_QUIT)
                {
                    exit(0);
                }

                // Pause/Play the game
                if(event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_p)
                {
                    play = !play;
                }

                // Check gravity
                if (play && SDL_GetTicks64() - tickBaseline >= gravityInterval)
                {
                    tickBaseline = SDL_GetTicks64();
                    stagingGrid = baseGrid;
                    lock = moveCheckRender(stagingGrid, &baseGrid, w, &piece1, stagingGrid.size(),q, 0, 1, false, true);
                }

                // Check keyboard input
                if (play && event.type == SDL_KEYDOWN)
                {
                    switch (event.key.keysym.sym)
                    {
                    case SDLK_LEFT:
                        stagingGrid = baseGrid;
                        moveCheckRender(stagingGrid, &baseGrid, w, &piece1, stagingGrid.size(), q, -1, 0);
                        break;
                    case SDLK_RIGHT:
                        stagingGrid = baseGrid;
                        moveCheckRender(stagingGrid,&baseGrid, w, &piece1, stagingGrid.size(), q, 1, 0);
                        break;
                    case SDLK_DOWN:
                        stagingGrid = baseGrid;
                        moveCheckRender(stagingGrid,&baseGrid, w, &piece1, stagingGrid.size(), q, 0, 1);
                        break;
                    case SDLK_UP:
                        piece1.rotate();
                        stagingGrid = baseGrid;
                        moveCheckRender(stagingGrid, &baseGrid,w, &piece1, stagingGrid.size(), q, 0, 0, true);
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
            score_to_add = baseGrid.clearLines();
            baseGrid.moveAllLinesDown();
            w.increment_score(score_to_add);
            score_to_add = 0;
            w.print_score();

            // generate next Shape
            pieceInformation = q.popAndGenerate();
            shapeNumber = pieceInformation.first;
            orientationNumber = pieceInformation.second;
            piece1 = piece(shapeNumber, orientationNumber);
        }

        time = std::chrono::system_clock::now();
        std::chrono::duration<double> lvl_time = time - time_last_lvl;

        if (lvl_time.count() > 5)
        {
            level++;
        }

        exit(0);
        std::cout << "level " << level << std::endl;
    }
    // TTF_Quit();
}
