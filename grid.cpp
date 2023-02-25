#include <iostream>

class grid 
{
    private:
        int gridWidth;
        int gridHeight;
        int padding;    
        int **gridArray;
    
    public:

        grid(int gridWidth = 10, int gridHeight = 20, int padding = 2)
        {
            // account for padding
            this->gridWidth = gridWidth + 2*padding;
            this->gridHeight = gridHeight + 2*padding;
            this->padding = padding;

            // initalise and populate the grid
            gridArray = new int*[this->gridWidth];
            for (int i = 0; i < this->gridWidth; i++)
            {
                gridArray[i] = new int[this->gridHeight];
                for (int j = 0; j < this->gridHeight; j++)
                {
                    if(i < padding || i >= this->gridWidth - padding || j < padding || j >= this->gridHeight - padding)
                    {
                        gridArray[i][j] = 9;
                    }
                    else
                    {
                        gridArray[i][j] = 0;
                    }
                }
            }
            
        }

        void print()
        {
            for (int j = 0; j < gridHeight; j++)
            {
                for (int i = 0; i < gridWidth; i++)
                {
                    std::cout << gridArray[i][j];
                }
                std::cout << std::endl;
            }
            std::cout << std::endl;
        }

        std::pair<int, int> boardToGrid(int x, int y)
        {
            // need to translate due to presence of padding for the walls
            return std::make_pair(x + padding, y + padding);
        }

        void update(int x, int y, int value)
        {
            // check if out of bounds
            if(x < 0 || y < 0 || x >= gridWidth || y >= gridHeight)
            {
                std::cout << "Out of bounds" << std::endl;
                return;
            }

            // perform translation of values
            std::pair<int, int> grid_pos = boardToGrid(x, y);

            // update the value
            gridArray[grid_pos.first][grid_pos.second] = value;
        }

        int** array()
        {
            return this->gridArray;
        }

        std::pair<int,int> size()
        {
            return std::make_pair(gridWidth, gridHeight);
        }  

};

class gameGrid : public grid
{
    
    public: 

        gameGrid(int gridWidth = 10, int gridHeight = 20, int padding = 2) : grid(gridWidth, gridHeight, padding)
        {
            // do nth
        }
        
        void updateWithPiece(std::pair<int,int> coords, int **pieceArray)
        {
            for(int i=0; i<4; i++)
            {
                for(int j=0; j<4; j++)
                {
                    if(pieceArray[j][i] != 0)
                    {   
                        update(coords.first + i, coords.second + j, pieceArray[j][i]);
                    }
                }
            }
            std::cout << std::endl;
        }

        
};
