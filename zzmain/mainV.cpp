// To compile code:
//    g++ renderer.cpp -o play -I include -L lib -l SDL2-2.0.0

#include <iostream>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include "shapes.hpp"

// Board parameters
#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 900  
#define SQUARE_WIDTH 40
#define padding 2

int boardWidth = 8;
int boardHeight = 15;


void setRenderColour(SDL_Renderer* renderer, int shapeNumber)
{
    SDL_SetRenderDrawColor(renderer, piecesColours[shapeNumber][0],piecesColours[shapeNumber][1],piecesColours[shapeNumber][2],piecesColours[shapeNumber][3]);
}

struct coordinates
{
    int x;
    int y;
};

coordinates calculateStartingPoint()
{
    // Calculate starting point for the grid
    int startingRectangleX = (WINDOW_WIDTH - boardWidth*SQUARE_WIDTH)/2;
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
    for (int j = 0; j < boardHeight - 2*padding; j++)
    {
        for (int i = 0; i < boardWidth - 2*padding; i++)
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

void printGrid(int **grid)
{
    // Print the board
    for (int j=0; j < boardHeight; j++)
    {
        for (int i=0; i < boardWidth; i++)
        {
            std::cout << grid[i][j] << " ";
        }
        std::cout << std::endl;
    }

}

int main()
{
    // Draw window
    SDL_Window* window = SDL_CreateWindow("Tetris", 10, 10, WINDOW_WIDTH, WINDOW_HEIGHT, false);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);
    SDL_Texture *backgroundTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB888,SDL_TEXTUREACCESS_TARGET, WINDOW_WIDTH, WINDOW_HEIGHT); 
    SDL_Texture *buffer = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB888,SDL_TEXTUREACCESS_TARGET, WINDOW_WIDTH, WINDOW_HEIGHT); 
    bool running = true;

    // We intend to always draw to the backbuffer, so we do not have to continuously redraw all previous frames
    SDL_SetRenderTarget(renderer, backgroundTexture); // all the draw calls go to backgroundTexture instead of the sdl backbuffer
    drawGrid(renderer);
    SDL_SetRenderTarget(renderer, NULL); // we set the renderer back so we can draw to the SDL backbuffer
    SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL); // copy our own backbuffer in one call
    SDL_RenderPresent(renderer);

    // Grid
    int **grid = new int*[boardWidth];
    for (int i=0; i<boardWidth; i++)
    {
        grid[i] = new int[boardHeight];
        for (int j=0; j<boardHeight; j++)
            if(i < padding || i >= boardWidth - padding || j < padding || j >= boardHeight - padding)
            {
                grid[i][j] = 9;
            } 
            else 
            {
                grid[i][j] = 0;
            }
    }
    printGrid(grid);

    // Starting coordinates
    coordinates pieceCoords;
    pieceCoords.x = 2;
    pieceCoords.y = 0;

    // Starting piece and orientation
    int shapeNumber = 0;
    int orientation = 0;

    // Gravity Interval
    int downMovementInterval = 500;
    int nextDownMovement = downMovementInterval + SDL_GetTicks64();

    while(running)
    {
        SDL_Event event;
        while(SDL_PollEvent(&event))
        {
            if(event.type == SDL_QUIT)
            {
                running = false;
            }

            // Load piece
            int incomingPiece[4][4];

            // Load the shape from shapes.hpp
            for (int i = 0; i < 4; i++)
            {
                for (int j = 0; j < 4; j++)
                {
                    incomingPiece[i][j] = pieces[shapeNumber][orientation][i][j];
                }
            }

            // Set colour
            setRenderColour(renderer, shapeNumber);

            // starting point
            coordinates startingPoint = calculateStartingPoint();

            // Draw the piece on the board
            for(int i=0; i<4; i++)
            {
                for(int j=0; j<4; j++)
                {
                    if(incomingPiece[i][j] == 1)
                    {
                        SDL_Rect incomingPieceRect;

                        incomingPieceRect.w = SQUARE_WIDTH;
                        incomingPieceRect.h = SQUARE_WIDTH;
                        incomingPieceRect.x = startingPoint.x + pieceCoords.x * incomingPieceRect.w + i * incomingPieceRect.w;
                        incomingPieceRect.y = startingPoint.y + pieceCoords.y * incomingPieceRect.w + j * incomingPieceRect.h;
                        
                        // Draw rectangle and fill it up
                        SDL_RenderDrawRect(renderer, &incomingPieceRect);
                        SDL_RenderFillRect(renderer, &incomingPieceRect);
                    }
                }
            }

            // Gravity implementation
            if(SDL_GetTicks64() >= nextDownMovement)
            {
                nextDownMovement = downMovementInterval + SDL_GetTicks64();
                // Move the piece down
                std::cout << "Move down" << std::endl;
            }





        }
    }



}
