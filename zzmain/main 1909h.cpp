// To compile code:
//    g++ renderer.cpp -o play -I include -L lib -l SDL2-2.0.0

#include <iostream>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include "shapes.hpp"

// Board parameters
#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 900  

// Grid parameters
#define SQUARE_WIDTH 30
#define BOARD_WIDTH 10
#define BOARD_HEIGHT 25
#define padding 2


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

coordinates boardSizeCheck(coordinates boardCoords)
{
    // If the piece is outside the board, we need to move it back inside
    if (boardCoords.x < -2)
        boardCoords.x = -2;
    if (boardCoords.x > BOARD_WIDTH - 3)
        boardCoords.x = BOARD_WIDTH - 3;
    if (boardCoords.y < -1)
        boardCoords.y = -1;
    if (boardCoords.y > BOARD_HEIGHT - 3)
        boardCoords.y = BOARD_HEIGHT - 3;

    return boardCoords;

}

coordinates keyPressCheck(coordinates initialBoardCoords)
{
    coordinates newBoardCoords;
    newBoardCoords = initialBoardCoords;

    // Check for keypresses and update the position of the piece
    const Uint8* state = SDL_GetKeyboardState(NULL);
    if (state[SDL_SCANCODE_LEFT])
        newBoardCoords.x--;
    if (state[SDL_SCANCODE_RIGHT])
        newBoardCoords.x++;
    if (state[SDL_SCANCODE_DOWN])
        newBoardCoords.y++;
    if (state[SDL_SCANCODE_UP])
        newBoardCoords.y--;

    // newBoardCoords = boardSizeCheck(newBoardCoords);

    return newBoardCoords;
}

void setRenderColour(SDL_Renderer* renderer, int shapeNumber)
{
    SDL_SetRenderDrawColor(renderer, piecesColours[shapeNumber][0],piecesColours[shapeNumber][1],piecesColours[shapeNumber][2],piecesColours[shapeNumber][3]);
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
    for (int j = 0; j < BOARD_HEIGHT; j++)
    {
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

void addPiece(SDL_Renderer* renderer, int shapeNumber, int orientation, coordinates coords, int grid[BOARD_WIDTH + 2*padding][BOARD_HEIGHT + 2*padding])
{
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
                incomingPieceRect.x = startingPoint.x + coords.x * incomingPieceRect.w + i * incomingPieceRect.w;
                incomingPieceRect.y = startingPoint.y + coords.y * incomingPieceRect.w + j * incomingPieceRect.h;
                
                // Draw rectangle and fill it up
                SDL_RenderDrawRect(renderer, &incomingPieceRect);
                SDL_RenderFillRect(renderer, &incomingPieceRect);
            }
        }
    }
}

void renderPiece(SDL_Renderer* renderer, SDL_Texture* backgroundTexture, SDL_Texture* buffer, coordinates boardCoords, int shapeNumber, int orientation, int grid[BOARD_WIDTH + 2*padding][BOARD_HEIGHT + 2*padding]) 
{
    SDL_SetRenderTarget(renderer, buffer); // all the draw calls go to buffer instead of the sdl backbuffer
    SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL); // copy our own backbuffer in one call
    addPiece(renderer, shapeNumber, orientation, boardCoords, grid);
    SDL_SetRenderTarget(renderer, NULL); // we set the renderer back so we can draw to the SDL backbuffer
    SDL_RenderCopy(renderer, buffer, NULL, NULL); // copy our own backbuffer in one call
    SDL_RenderPresent(renderer);        
}

void printGrid(int grid[BOARD_WIDTH + 2*padding][BOARD_HEIGHT + 2*padding])
{
    // Print the board
    for (int j=0; j < BOARD_HEIGHT + 2*padding; j++)
    {
        for (int i=0; i < BOARD_WIDTH + 2*padding; i++)
        {
            std::cout << grid[i][j] << " ";
        }
        std::cout << std::endl;
    }

}

int** updateGrid(int **grid, coordinates boardCoords, int shapeNumber, int orientation)
{
    // initalise incomingPiece
    int incomingPiece[4][4];

    // initalise grid to ensure they can be returned
    int** originalGrid = new int*[BOARD_HEIGHT + 2*padding];
    for (int i=0; i<BOARD_HEIGHT + 2*padding; i++)
    {
        originalGrid[i] = new int[BOARD_WIDTH + 2*padding];
    }

    int** newGrid = new int*[BOARD_HEIGHT + 2*padding];
    for (int i=0; i<BOARD_HEIGHT + 2*padding; i++)
    {
        newGrid[i] = new int[BOARD_WIDTH + 2*padding];
    }

    // We need to check if the move is valid
    memcpy(originalGrid, grid, sizeof (int) * (BOARD_WIDTH + 2*padding) * (BOARD_HEIGHT + 2*padding));
    memcpy(newGrid, grid, sizeof (int) * (BOARD_WIDTH + 2*padding) * (BOARD_HEIGHT + 2*padding));

    // Load the shape from shapes.hpp
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            incomingPiece[i][j] = pieces[shapeNumber][orientation][i][j];
            if (incomingPiece[i][j] == 1)
                newGrid[boardCoords.x + i + padding][boardCoords.y + j + padding] += 1;
        }
    }

    // Check if the piece is outside the board
    // Equivalent to checking for 2s on the newGrid
    for (int j=0; j<BOARD_HEIGHT + 2*padding; j++)
    {
        for (int i=0; i<BOARD_WIDTH + 2*padding; i++)
        {
            if (newGrid[i][j] == 2)
            {
                return newGrid;
            }
        }
    }

    return originalGrid;
}

void renderOnePiece(SDL_Renderer* renderer, SDL_Texture* backgroundTexture, SDL_Texture* buffer, int grid[BOARD_WIDTH + 2*padding][BOARD_HEIGHT + 2*padding])
{
    coordinates boardCoords;
    boardCoords.x = 0;
    boardCoords.y = 0;

    int shapeNumber = 0;
    int orientation = 4;

    coordinates oldBoardCoords;
    int incomingPiece[4][4];
    int newGrid[BOARD_WIDTH + 2*padding][BOARD_HEIGHT + 2*padding];

    // Launch the window
    bool running = true;

    int downMovementInterval = 500;
    int nextDownMovement = downMovementInterval + SDL_GetTicks64();

    while (running)
    {
        SDL_Event event;
        while(SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
                running = false;
        }

        // Get old coordinates
        oldBoardCoords = boardCoords;

        // Gravity implementation
        if(SDL_GetTicks64() >= nextDownMovement)
        {
            boardCoords.y += 1;
            nextDownMovement = SDL_GetTicks64() + downMovementInterval;
            if(updateGrid(grid, boardCoords, shapeNumber, orientation)==false)
            {
                boardCoords = oldBoardCoords;
                std::cout << "should exit here" << std::endl;   
            }    
        } 
        else 
        {
            // Check for keypresses and update the position of the piece
            boardCoords = keyPressCheck(boardCoords);   
            if(updateGrid(grid, boardCoords, shapeNumber, orientation)==false)
                boardCoords = oldBoardCoords;
        }

        SDL_SetRenderTarget(renderer, buffer); // all the draw calls go to buffer instead of the sdl backbuffer
        SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL); // copy our own backbuffer in one call
        addPiece(renderer, shapeNumber, orientation, boardCoords, grid);
        SDL_SetRenderTarget(renderer, NULL); // we set the renderer back so we can draw to the SDL backbuffer
        SDL_RenderCopy(renderer, buffer, NULL, NULL); // copy our own backbuffer in one call
        SDL_RenderPresent(renderer);      

    } 
}


int main(int argc, char** argv)
{
    // We need to define the back-end grid
    // Defining the back-end grid
    int grid[BOARD_WIDTH + 2*padding][BOARD_HEIGHT + 2*padding];

    // Initalise the grid
    for(int j=0; j<BOARD_HEIGHT + 2*padding; j++)
    {
        for(int i=0; i<BOARD_WIDTH + 2*padding; i++)
        {
            if(i < padding || i >= BOARD_WIDTH + padding || j < padding || j >= BOARD_HEIGHT + padding)
                grid[i][j] = 1;
            else
                grid[i][j] = 0;
        }
    }

    // Draw window
    SDL_Window* window = SDL_CreateWindow("A line", 10, 10, WINDOW_WIDTH, WINDOW_HEIGHT, false);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);
    SDL_Texture *backgroundTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB888,SDL_TEXTUREACCESS_TARGET, WINDOW_WIDTH, WINDOW_HEIGHT); 
    SDL_Texture *cumulative = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB888,SDL_TEXTUREACCESS_TARGET, WINDOW_WIDTH, WINDOW_HEIGHT); 
    SDL_Texture *buffer = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB888,SDL_TEXTUREACCESS_TARGET, WINDOW_WIDTH, WINDOW_HEIGHT); 

    // We intend to always draw to the backbuffer, so we do not have to continuously redraw all previous frames
    SDL_SetRenderTarget(renderer, backgroundTexture); // all the draw calls go to backgroundTexture instead of the sdl backbuffer
    drawGrid(renderer);
    SDL_SetRenderTarget(renderer, NULL); // we set the renderer back so we can draw to the SDL backbuffer
    SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL); // copy our own backbuffer in one call
    SDL_RenderPresent(renderer);
    printGrid(grid);


    for (int i=0;i<3;i++)
    {
        std::cout << "Iteration " << i << std::endl;
        renderOnePiece(renderer, backgroundTexture, buffer, grid);


    }
    

    SDL_Quit();
    return 0;
}