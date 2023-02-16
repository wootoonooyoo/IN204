/* 
To compile code
    g++ renderer.cpp -o play -I include -L lib -l SDL2-2.0.0

In this code, we would like to display the board for a given height and width.
Subsequently, given a 4 x 4-tuple of integers, we would like to display the corresponding piece.
*/

#include <SDL2/SDL.h>
#include <iostream>


// Function to launch a window
void launchWindow(){

    // Initalise variables for window width and height
    int windowWidth = 1200;
    int windowHeight = 900;

    // Draw window
    SDL_Window* window = SDL_CreateWindow("A line", 10, 10, windowWidth, windowHeight, false);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);

    // Launch the window
    bool running = true;

    while (running)
    {

        SDL_Event event;
        while( SDL_PollEvent(&event) )
        {
            if (event.type == SDL_QUIT)
                running = false;
        }

        // Set the background
        SDL_SetRenderDrawColor(renderer, 122, 122, 122, 122);

        // Define base variables
        int squareWidth = 30;
        int boardWidth = 10;
        int boardHeight = 24;

        // Calculate starting point for the grid
        int startingRectangleX = (windowWidth - boardWidth*squareWidth)/2;
        int startingRectangleY = 10;

        // Draw the base grid
        for (int j = 0; j < boardHeight; j++){
            for (int i = 0; i < boardWidth; i++)
            {
                SDL_Rect dstrect;
                dstrect.w = squareWidth;
                dstrect.h = squareWidth;
                dstrect.x = startingRectangleX + dstrect.w*i;
                dstrect.y = startingRectangleY + dstrect.h*j;

                SDL_RenderDrawRect(renderer, &dstrect);
            }

        }

        // Render
        SDL_RenderPresent(renderer);
    }

    SDL_Quit();

}


// Update the window

int main(int argc, char** argv)
{

    launchWindow();

    SDL_Window* window = SDL_CreateWindow("A line", 10, 10, windowWidth, windowHeight, false);

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);

    SDL_Rect incomingPieceRect;

    incomingPieceRect.w = 30;
    incomingPieceRect.h = 30;
    incomingPieceRect.x = 30
    incomingPieceRect.y = 30;

    
    // Draw rectangle and fill it up
    SDL_RenderDrawRect(renderer, &incomingPieceRect);
    SDL_RenderFillRect(renderer, &incomingPieceRect);

    return 0;
}