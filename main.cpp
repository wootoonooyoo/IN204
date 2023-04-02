#include <iostream>
#include <array>
#include <random>
#include <cstdbool>
#include "grid.cpp"
#include "piece.hpp"
#include "window.cpp"
#include <chrono>

void moveCheckRender(gameGrid stagingGrid, gameWindow w, piece *piece1, std::pair<int, int> boardSize, int x_change = 0, int y_change = 0, bool isRotation = false)
{
    // Move
    piece1->move(x_change, y_change);

    // Check
    if (!stagingGrid.checkMoveValidity(*piece1))
    {
        if (isRotation)
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
    w.renderGrid(stagingGrid.array(), boardSize); // base layer which contains the wall and pieces
    w.drawGrid(stagingGrid.size());               // grid for aesthetic reasons
    w.render();                                   // render the window
    // Prepare next iteration
    stagingGrid.resetGrid();
    w.clearStagingBuffer();
}

bool moveCheckLockRender(gameGrid stagingGrid, gameGrid *baseGrid, gameWindow w, piece *piece1, std::pair<int, int> boardSize, int x_change = 0, int y_change = 0)
{
    // Move
    piece1->move(x_change, y_change);

    // Check
    if (!stagingGrid.checkMoveValidity(*piece1))
    {
        // Lock
        return true;
    }

    // Render
    stagingGrid.updateWithPiece(*piece1);
    w.renderGrid(stagingGrid.array(), boardSize); // base layer which contains the wall and pieces
    w.drawGrid(stagingGrid.size());               // grid for aesthetic reasons
    w.render();                                   // render the window
    w.renderScore();
    // Prepare next iteration
    stagingGrid.resetGrid();
    w.clearStagingBuffer();

    return false;
}

int rngGenerator(int min = 0, int max = 6)
{
    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<int> uni(min, max);
    return uni(rng);
}

int main()
{
    TTF_Init();
    auto time_last_lvl = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::now();
    std::chrono::duration<double> lvl_duration;
    std::cout << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << std::endl;
    std::cout << "@@@@@@@@@@@@@@@@@@@@ Tetris Game @@@@@@@@@@@@@@@@@@@@" << std::endl;
    std::cout << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << std::endl;
    std::cout << "Appuyez sur p pour lancer le jeu" << std::endl;
    std::cout << "Appuyez sur s pour mettre en pause" << std::endl;

    while (true)
    {
        // Generate
        SDL_Event event;
        gameGrid stagingGrid(10, 20, 4);
        gameGrid baseGrid(10, 20, 4);
        piece piece1(3, 3, 1, 0);
        gameWindow w(100, 100, 900, 900);
        std::pair<int, int> coords{6, 1};
        int level = 0;
        int gravityInterval = 500; // ms
        int tickBaseline = 0;      // ms
        int gravityCount = 0;
        bool lock = false;
        int linescleared = 0;

        int shapeNumber = rngGenerator(0, 6);
        int orientationNumber = rngGenerator(0, 3);

        moveCheckRender(stagingGrid, w, &piece1, stagingGrid.size());
        tickBaseline = SDL_GetTicks64() - gravityInterval;

        // Play/pause feature
        bool play = false;

        for (int i = 0; i < 100; i++)
        {

            while (!lock)
            {

                // Quit the programme
                if (SDL_PollEvent(&event) && event.type == SDL_QUIT)
                {
                    exit(0);
                }

                if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_p)
                {
                    if (!play)
                        std::cout << "Jeu repris, appuyez sur s pour mettre en pause" << std::endl;
                    play = true;
                }

                if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_s)
                {
                    if (play)
                        std::cout << "Jeu mis en pause, appuyez sur p pour reprendre" << std::endl;
                    play = false;
                }

                // Check gravity
                if (play && (SDL_GetTicks64() - tickBaseline >= gravityInterval))
                {
                    tickBaseline = SDL_GetTicks64();
                    stagingGrid = baseGrid;
                    lock = moveCheckLockRender(stagingGrid, &baseGrid, w, &piece1, stagingGrid.size(), 0, 1);
                }

                // Check keyboard input
                if (play && event.type == SDL_KEYDOWN)
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
                if (level < 15)
                {
                    time = std::chrono::system_clock::now();
                    lvl_duration = time - time_last_lvl;
                    if (lvl_duration.count() > 15)
                    {
                        time_last_lvl = time;
                        level++;
                        std::cout << "Niveau " << level << std::endl;
                        gravityInterval *= 0.85;
                    }
                }
            }

            // save into baseGrid
            piece1.revert();
            baseGrid.updateWithPiece(piece1);
            lock = false;

            // check if there are any lines to clear
            linescleared = baseGrid.clearLines();
            baseGrid.moveAllLinesDown();
            std::cout << linescleared << std::endl;
            switch (linescleared)
            {
            case 1:
                std::cout << "1 ligne supprimée !" << std::endl;
                w.increment_score(40);
            case 2:
                std::cout << "2 lignes supprimées !" << std::endl;
                w.increment_score(100);
            case 3:
                std::cout << "3 lignes supprimées !" << std::endl;
                w.increment_score(300);
            case 4:
                std::cout << "4 lignes supprimées ! Tetris :" << std::endl;
                w.increment_score(1200);
            }
            linescleared = 0;
            // generate next Shape
            shapeNumber = rngGenerator(0, 6);
            orientationNumber = rngGenerator(0, 3);
            piece1 = piece(shapeNumber, orientationNumber, 3, 0);
        }
        exit(0);
    }
    TTF_Quit();
}
