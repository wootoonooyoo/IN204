#include <string>
#include <iostream>
#include <SDL2/SDL.h>
#include <tuple>

class window
{
    // we want to initalise a SDL window with renderers, textures, etc
    private:

        SDL_Window *gameWindow;
        SDL_Renderer *renderer;
        SDL_Texture *backgroundGrid;
        SDL_Texture *buffer;

        int width;
        int height;
        int squareSize;
        
    public:

        window(int x_pos, int y_pos, int width, int height)
        {   
            // Create window, renderer, and textures
            this->gameWindow = SDL_CreateWindow("test", x_pos, y_pos, width, height, 0);
            this->renderer = SDL_CreateRenderer(gameWindow, -1, 0);
            this->backgroundGrid = SDL_CreateTexture(renderer,SDL_PIXELFORMAT_RGB888,SDL_TEXTUREACCESS_TARGET, width, height); 
            this->buffer = SDL_CreateTexture(renderer,SDL_PIXELFORMAT_RGB888,SDL_TEXTUREACCESS_TARGET, width, height); 
        
            this->width = width;
            this->height = height;
            this->squareSize = 30;
        }

        void render()
        {
            // Render the textures
            SDL_RenderCopy(renderer, this->backgroundGrid, NULL, NULL);
            SDL_RenderCopy(renderer, this->buffer, NULL, NULL);
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

        void drawLine(int x1, int y1, int x2, int y2)
        {
            // Set the colour of the line
            SDL_SetRenderDrawColor(this->renderer, 0, 0, 0, 255);

            // Draw a line
            SDL_SetRenderTarget(renderer, this->buffer);
            SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
            SDL_SetRenderTarget(renderer, NULL);
        }

        void drawRectangle(int x_pos, int y_pos, std::tuple<int,int,int,int> colour = std::make_tuple(100,100,100,255))
        {
            // Set the colour of the rectangle
            SDL_SetRenderDrawColor(renderer, std::get<0>(colour), std::get<1>(colour), std::get<2>(colour), std::get<3>(colour));

            // Draw a rectangle
            SDL_Rect incomingPieceRect;

            incomingPieceRect.w = this->squareSize;
            incomingPieceRect.h = this->squareSize;
            incomingPieceRect.x = x_pos;
            incomingPieceRect.y = y_pos;
            
            // Draw rectangle and fill it up
            SDL_SetRenderTarget(renderer, this->buffer);
            SDL_RenderDrawRect(renderer, &incomingPieceRect);
            SDL_RenderFillRect(renderer, &incomingPieceRect);
            SDL_SetRenderTarget(renderer, NULL);

        }

        void clearBuffer()
        {
            // Clear the buffer
            SDL_SetRenderTarget(renderer, this->buffer);
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);
            SDL_SetRenderTarget(renderer, NULL);
        }
    

        friend class gameWindow;

};

class gameWindow : public window
{
    public:

        gameWindow(int x_pos, int y_pos, int width, int height) : window(x_pos, y_pos, width, height)
        {
            // Create the game window
        }

        void drawGrid(std::pair<int, int> boardSize)
        {
            // Draw the grid
            for (int i = 0; i < boardSize.first; i++)
            {
                drawLine(i*30, 0, i*30, boardSize.second*30);
            }
            
            for (int i = 0; i < boardSize.second; i++)
            {
                drawLine(0, i*30, boardSize.first*30, i*30);
            }
        }

        void renderGrid(int **gridArray)
        {
            // determine the size of the grid
            int gridWidth = 14;
            int gridHeight = 24;

            // render grid
            for (int i = 0; i < gridWidth; i++)
            {
                for (int j = 0; j < gridHeight; j++)
                {
                    if(gridArray[i][j] != 0)
                    {
                        // get colour for rectangle
                        std::tuple<int,int,int,int> colour = pieceColour(gridArray[i][j]);

                        // draw rectangle
                        drawRectangle(i*30, j*30, colour);
                    }
                }
            }


        }

};

// int main()
// {
//     gameWindow w(100, 100, 800, 640);
//     // w.drawRectangle(100,100);
//     // w.drawRectangle(200,200);

//     // w.render();
    

//     int **test = new int*[10];
//     for (int i = 0; i < 10; i++)
//     {
//         test[i] = new int[10];
//         for (int j = 0; j < 10; j++)
//         {
//             if((i+j)%2 == 0)
//             {
//                 test[i][j] = 1;
//             } else 
//             {
//                 test[i][j] = 0;
//             }
//         }
//     }

//     //print
//     for (int j = 0; j < 10; j++)
//     {
//         for (int i = 0; i < 10; i++)
//         {
//             std::cout << test[i][j];
//         }
//         std::cout << std::endl;
//     }

//     w.renderGrid(test);
//     w.render();

// }