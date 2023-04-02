#include <iostream>
#include <array>
#include <random>
#include <cstdbool>
#include "grid.cpp"
#include "piece.hpp"
#include "window.cpp"
#include <chrono>

bool moveCheckRender(gameGrid stagingGrid, gameGrid *baseGrid, gameWindow w, piece *piece1, std::pair<int, int> boardSize, queue q, int x_change = 0, int y_change = 0, bool isRotation = false, bool isGravityTriggered = false)
{
    // Move
    piece1->move(x_change, y_change);

    // Check
    if (isGravityTriggered && !stagingGrid.checkMoveValidity(*piece1))
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
    w.renderGrid(stagingGrid.array(), boardSize, stagingGrid.boardPadding()); // base layer which contains the wall and pieces
    w.drawGrid(stagingGrid.size());               // grid for aesthetic reasons
    w.renderQueue(q);                             // queue
    w.render();                                   // render the window

    // Prepare next iteration
    stagingGrid.resetGrid();
    w.clearStagingBuffer();

    return false;
}
int main()
{
    std::cout << "testV2" << std::endl;

    auto time_last_lvl = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::now();
    std::chrono::duration<double> lvl_duration;
    std::cout << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << std::endl;
    std::cout << "@@@@@@@@@@@@@@@@@@@@ Tetris Game @@@@@@@@@@@@@@@@@@@@" << std::endl;
    std::cout << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << std::endl;
    std::cout << "Appuyez sur p pour lancer le jeu" << std::endl;
    std::cout << "Appuyez sur s pour mettre en pause" << std::endl;
    std::cout << "Pour déplacer la pièce appuyez sur les flèches gauche ou droite" << std::endl;
    std::cout << "Pour faire tourner la pièce appuyez sur la flèche du haut" << std::endl;
    std::cout << "Pour accélérer la chute appuyez sur la flèche du bas" << std::endl;

    while (true)
    {
        // Generate
        SDL_Event event;
        gameGrid stagingGrid(10, 20, 4);
        gameGrid baseGrid(10, 20, 4);
        piece piece1(3, 3);
        gameWindow w(100, 100, 900, 900);
        std::pair<int, int> coords{6, 1};
        int level = 0;
        int gravityInterval = 500; // ms
        int tickBaseline = 0;      // ms
        int gravityCount = 0;
        bool lock = false;
        int linescleared = 0;
        bool gameOver = false;

        queue q(3);
        q.generateMax();
        std::pair<int, int> pieceInformation = q.popAndGenerate();
        /*
        // Game over Check
        if (!movePossible)
        {
            std::cout << "Game Over ! Score : " << w.get_score() << std::endl
                      << "Appuyez sur p pour relancer !" << std::endl;
            gameOver = true;
        }*/

        // Render first piece
        int shapeNumber = pieceInformation.first;
        int orientationNumber = pieceInformation.second;
        piece1 = piece(shapeNumber, orientationNumber);
        moveCheckRender(stagingGrid, &baseGrid, w, &piece1, stagingGrid.size(), q);

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
                    if (!gameOver)
                        gameOver = false;
                }

                if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_s)
                {
                    if (play)
                        std::cout << "Jeu mis en pause, appuyez sur p pour reprendre" << std::endl;
                    play = false;
                }

                // Check gravity
                if (!gameOver && play && (SDL_GetTicks64() - tickBaseline >= gravityInterval))
                {
                    tickBaseline = SDL_GetTicks64();
                    stagingGrid = baseGrid;
                    lock = moveCheckRender(stagingGrid, &baseGrid, w, &piece1, stagingGrid.size(), q, 0, 1, false, true);
                }

                // Check keyboard input
                if (!gameOver && play && event.type == SDL_KEYDOWN)
                {
                    switch (event.key.keysym.sym)
                    {
                    case SDLK_LEFT:
                        stagingGrid = baseGrid;
                        moveCheckRender(stagingGrid, &baseGrid, w, &piece1, stagingGrid.size(), q, -1, 0);
                        break;
                    case SDLK_RIGHT:
                        stagingGrid = baseGrid;
                        moveCheckRender(stagingGrid, &baseGrid, w, &piece1, stagingGrid.size(), q, 1, 0);

                        break;
                    case SDLK_DOWN:
                        stagingGrid = baseGrid;
                        moveCheckRender(stagingGrid, &baseGrid, w, &piece1, stagingGrid.size(), q, 0, 1);
                        break;
                    case SDLK_UP:
                        piece1.rotate();
                        stagingGrid = baseGrid;
                        moveCheckRender(stagingGrid, &baseGrid, w, &piece1, stagingGrid.size(), q, 0, 0, true);

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
                        gravityInterval *= 0.85;
                        std::cout << "Niveau " << level << std::endl;
                    }
                }
            }

            // save into baseGrid
            piece1.revert();
            baseGrid.updateWithPiece(piece1);
            lock = false;

            // check if there are any lines to clear
            baseGrid.clearLines(&linescleared);
            baseGrid.moveAllLinesDown();
            if (linescleared == 4)
            {

                std::cout << "4 lignes supprimées ! Tetris :" << std::endl;
                w.increment_score(1200);
                std::cout << "Score : " << w.get_score() << std::endl;
            }
            if (linescleared == 3)
            {
                std::cout << "3 lignes supprimées !" << std::endl;
                w.increment_score(300);
                std::cout << "Score : " << w.get_score() << std::endl;
            }
            if (linescleared == 2)
            {
                std::cout << "2 lignes supprimées !" << std::endl;
                w.increment_score(100);
                std::cout << "Score : " << w.get_score() << std::endl;
            }
            if (linescleared == 1)
            {

                std::cout << "1 ligne supprimée !" << std::endl;
                w.increment_score(40);
                std::cout << "Score : " << w.get_score() << std::endl;
            }
            linescleared = 0;

            // generate next Shape
            pieceInformation = q.popAndGenerate();
            shapeNumber = pieceInformation.first;
            orientationNumber = pieceInformation.second;
            piece1 = piece(shapeNumber, orientationNumber);
        }
        exit(0);
    }
}
