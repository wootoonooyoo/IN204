/* 
To compile code:
    g++ renderer.cpp -o play -I include -L lib -l SDL2-2.0.0

In this code, we would like to display the board for a given height and width.
Subsequently, given a 4 x 4-tuple of integers, we would like to display the corresponding piece.
*/

#include <iostream>
#include <SDL2/SDL.h>
#include "shapes.hpp"

// Board parameters
#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 900  

// Grid parameters
#define SQUARE_WIDTH 30
#define BOARD_WIDTH 10
#define BOARD_HEIGHT 24

void setRenderColour(SDL_Renderer* renderer, int shapeNumber, int i)
{
    SDL_SetRenderDrawColor(renderer, piecesColours[shapeNumber][0] + i,piecesColours[shapeNumber][1] + i,piecesColours[shapeNumber][2] + i,piecesColours[shapeNumber][3] + i);
}

struct coordinates
{
    int x;
    int y;
};

coordinates calculateStartingPoint()
{
    // Calculate starting point for the grid
    int startingRectangleX = (WINDOW_WIDTH - BOARD_WIDTH*SQUARE_WIDTH)/2;
    int startingRectangleY = 10;

    coordinates startingPoint;
    startingPoint.x = startingRectangleX;
    startingPoint.y = startingRectangleY;

    return startingPoint;
}

void drawGrid(SDL_Renderer* renderer)
{
    // Set the background
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 122, 122, 122, 122);

    // Calculate starting point for the grid
    coordinates startingPoint = calculateStartingPoint();

    // Draw the base grid
    for (int j = 0; j < BOARD_HEIGHT; j++){
        for (int i = 0; i < BOARD_WIDTH; i++)
        {
            SDL_Rect dstrect;
            dstrect.w = SQUARE_WIDTH;
            dstrect.h = SQUARE_WIDTH;
            dstrect.x = startingPoint.x + dstrect.w*i;
            dstrect.y = startingPoint.y + dstrect.h*j;

            SDL_RenderDrawRect(renderer, &dstrect);
        }

    }

}

void addPiece(SDL_Renderer* renderer, int shapeNumber, int orientation, int board_X, int board_Y, int i)
{
    int incomingPiece[4][4];

    // Load the shape from shapes.hpp
    for (int i = 0; i < 4; i++){
        for (int j = 0; j < 4; j++){
            incomingPiece[i][j] = pieces[shapeNumber][orientation][i][j];
        }
    }

    // Set colour
    setRenderColour(renderer, shapeNumber, i);

    // starting point
    coordinates startingPoint = calculateStartingPoint();

    // Draw the piece on the board
    for(int i=0; i<4; i++){
        for(int j=0; j<4; j++){
            if(incomingPiece[j][i] == 1){

                SDL_Rect incomingPieceRect;

                incomingPieceRect.w = SQUARE_WIDTH;
                incomingPieceRect.h = SQUARE_WIDTH;
                incomingPieceRect.x = startingPoint.x + board_X * incomingPieceRect.w + i * incomingPieceRect.w;
                incomingPieceRect.y = startingPoint.y + board_Y * incomingPieceRect.w + j * incomingPieceRect.h;
                
                // Draw rectangle and fill it up
                SDL_RenderDrawRect(renderer, &incomingPieceRect);
                SDL_RenderFillRect(renderer, &incomingPieceRect);
            }
        }
    }

}


int main(int argc, char** argv)
{
    // Draw window
    SDL_Window* window = SDL_CreateWindow("A line", 10, 10, WINDOW_WIDTH, WINDOW_HEIGHT, false);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);

    // Launch the window
    bool running = true;
    int i = 1;
    int j = 0;
    int shapeNumber = 4;
    int orientation = 4;

    while (running)
    {
        SDL_Event event;
        while( SDL_PollEvent(&event) )
        {
            if (event.type == SDL_QUIT)
                running = false;
        }

        drawGrid(renderer);
        addPiece(renderer, shapeNumber, orientation, 2, j, i);
        SDL_RenderPresent(renderer);

    }

    SDL_Quit();
    return 0;
}