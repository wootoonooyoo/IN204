#include <string>
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <tuple>

class window
{
    // we want to initalise a SDL window with renderers, textures, etc
private:
    SDL_Window *gameWindow;
    SDL_Renderer *renderer;
    SDL_Texture *commitBuffer;
    SDL_Texture *stagingBuffer;
    TTF_Font *score_font;

    SDL_Surface *score_surf;
    SDL_Color score_color;
    SDL_Texture *score_texture;
    SDL_Rect score_loc;

    int width;
    int height;
    int squareSize;
    int score = 0;

public:
    window(int x_pos, int y_pos, int width, int height)
    {
        // Create window, renderer, and textures
        this->gameWindow = SDL_CreateWindow("Tetris", x_pos, y_pos, width, height, 0);
        this->renderer = SDL_CreateRenderer(gameWindow, -1, 0);
        this->commitBuffer = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB888, SDL_TEXTUREACCESS_TARGET, width, height);
        this->stagingBuffer = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB888, SDL_TEXTUREACCESS_TARGET, width, height);
        this->score_color = {255, 100, 70};
        this->score_font = TTF_OpenFont("font/OpenSans-ExtraBold.ttf", 10);
        this->width = width;
        this->height = height;
        this->squareSize = 30;
    }

    void render()
    {
        // Render the textures
        // SDL_RenderCopy(renderer, this->commitBuffer, NULL, NULL);
        SDL_RenderCopy(renderer, this->stagingBuffer, NULL, NULL);
        SDL_RenderPresent(renderer);
    }

    void clearStagingBuffer()
    {
        // Clear the texture by filling it up with black
        drawRectangle(0, 0, this->width, this->height, std::make_tuple(0, 0, 0, 255));
    }

    void drawLine(int x1, int y1, int x2, int y2)
    {
        // Set the colour of the line
        SDL_SetRenderDrawColor(this->renderer, 0, 0, 0, 255);

        // Draw a line
        SDL_SetRenderTarget(renderer, this->stagingBuffer);
        SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
        SDL_SetRenderTarget(renderer, NULL);
    }

    void drawRectangle(int x_pos, int y_pos, int rec_width, int rec_height, std::tuple<int, int, int, int> colour = std::make_tuple(100, 100, 100, 255))
    {
        // Set the colour of the rectangle
        SDL_SetRenderDrawColor(renderer, std::get<0>(colour), std::get<1>(colour), std::get<2>(colour), std::get<3>(colour));

        // Draw a rectangle
        SDL_Rect incomingPieceRect;

        incomingPieceRect.w = rec_width;
        incomingPieceRect.h = rec_height;
        incomingPieceRect.x = x_pos;
        incomingPieceRect.y = y_pos;

        // Draw rectangle and fill it up
        SDL_SetRenderTarget(renderer, this->stagingBuffer);
        SDL_RenderDrawRect(renderer, &incomingPieceRect);
        SDL_RenderFillRect(renderer, &incomingPieceRect);
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
            drawLine(i * 30, 0, i * 30, boardSize.second * 30);
        }

        for (int i = 0; i < boardSize.second; i++)
        {
            drawLine(0, i * 30, boardSize.first * 30, i * 30);
        }
    }

    void increment_score(int val) { this->score += val; }
    void print_score() const { std::cout << this->score << std::endl; }
    void renderGrid(int **gridArray, std::pair<int, int> boardSize)
    {
        // determine the size of the grid
        int gridWidth = boardSize.first;
        int gridHeight = boardSize.second;

        // render grid
        for (int i = 0; i < gridWidth; i++)
        {
            for (int j = 0; j < gridHeight; j++)
            {
                if (gridArray[i][j] != 0)
                {
                    // get colour for rectangle
                    std::tuple<int, int, int, int> colour = pieceColour(gridArray[i][j]);

                    // draw rectangle
                    drawRectangle(i * 30, j * 30, this->squareSize, this->squareSize, colour);
                }
            }
        }
    }
    void renderScore()
    {
        score_surf = TTF_RenderUTF8_Blended(score_font, "Score : " + char(score), score_color);
        score_texture = SDL_CreateTextureFromSurface(renderer, score_surf);
        score_loc = {100, 100, 500, 500};
        SDL_RenderCopy(renderer, score_texture, &score_loc, &score_loc);
    }
};
