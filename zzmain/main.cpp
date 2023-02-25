#include <iostream>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include "shapes.hpp"

int main()
{

    // create a window, renderer, and draw one line, render it, and wait 3 seconds
    SDL_Window *window = SDL_CreateWindow("SDL2", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, 0);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderDrawLine(renderer, 0, 0, 640, 480);
    SDL_RenderPresent(renderer);
    
    while(true)
    {
        SDL_Event event;
        if(SDL_PollEvent(&event) && event.type == SDL_QUIT)
        {
            break;
        }
    }
    
    

}