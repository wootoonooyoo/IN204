#include <iostream>
#include "piece.cpp"

/*
The class grid intends to create a 2D array of integers, which will be used to represent the game board.
The grid will be padded with a border of 9s.
*/

// Class grid merely creates a 2D table
class grid
{
public:
    int gridWidth;
    int gridHeight;
    int padding;
    int **gridArray;

    grid(int gridWidth = 10, int gridHeight = 20, int padding = 0)
    {
        this->gridWidth = gridWidth + 2 * padding;
        this->gridHeight = gridHeight + 2 * padding;
        this->padding = padding;

        // initalise and populate the grid
        gridArray = new int *[this->gridWidth];
        for (int i = 0; i < this->gridWidth; i++)
        {
            gridArray[i] = new int[this->gridHeight];
            for (int j = 0; j < this->gridHeight; j++)
            {
                gridArray[i][j] = 0;
            }
        }
    }

    // Print Methods
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

    // Return Methods
    int **array()
    {
        return this->gridArray;
    }

    std::pair<int, int> size()
    {
        return std::make_pair(gridWidth, gridHeight);
    }

    int boardPadding()
    {
        return this->padding;
    }
    // Update Methods
    void update(int x, int y, int value)
    {
        this->gridArray[x][y] = value;
    }

    void update(std::pair<int, int> coords, int value)
    {
        this->gridArray[coords.first][coords.second] = value;
    }

    // Flatten grid
    void flattenGrid()
    {
        for (int i = 0; i < gridWidth; i++)
        {
            for (int j = 0; j < gridHeight; j++)
            {
                if (gridArray[i][j] != 0)
                {
                    gridArray[i][j] = 1;
                }
            }
        }
    }

    // Operator = overload
    grid &operator=(const grid &other)
    {
        if (this != &other) // self-assignment check expected
        {
            // 1: allocate new memory and copy the elements
            int **newGridArray = new int *[other.gridWidth];
            for (int i = 0; i < other.gridWidth; i++)
            {
                newGridArray[i] = new int[other.gridHeight];
                for (int j = 0; j < other.gridHeight; j++)
                {
                    newGridArray[i][j] = other.gridArray[i][j];
                }
            }

            // 2: deallocate old memory
            for (int i = 0; i < gridWidth; i++)
            {
                delete[] gridArray[i];
            }
            delete[] gridArray;

            // 3: assign the new memory to the object
            gridArray = newGridArray;
            gridWidth = other.gridWidth;
            gridHeight = other.gridHeight;
            padding = other.padding;
        }
        // by convention, always return *this
        return *this;
    }

    // Operator += overload
    grid &operator+=(const grid &other)
    {
        if (this != &other) // self-assignment check expected
        {
            // 1: allocate new memory and copy the elements
            int **newGridArray = new int *[other.gridWidth];
            for (int i = 0; i < other.gridWidth; i++)
            {
                newGridArray[i] = new int[other.gridHeight];
                for (int j = 0; j < other.gridHeight; j++)
                {
                    newGridArray[i][j] = gridArray[i][j] + other.gridArray[i][j];
                }
            }

            // 2: deallocate old memory
            for (int i = 0; i < gridWidth; i++)
            {
                delete[] gridArray[i];
            }
            delete[] gridArray;

            // 3: assign the new memory to the object
            gridArray = newGridArray;
            gridWidth = other.gridWidth;
            gridHeight = other.gridHeight;
            padding = other.padding;
        }
        // by convention, always return *this
        return *this;
    }

    // Reset Method
    void resetGrid()
    {
        for (int i = 0; i < gridWidth; i++)
        {
            for (int j = 0; j < gridHeight; j++)
            {
                gridArray[i][j] = 0;
            }
        }
    }
};

// In this gameGrid, we intend to create a grid with padding
class gameGrid : public grid
{
public:
    gameGrid(int boardWidth = 10, int boardHeight = 20, int boardPadding = 3) : grid(boardWidth, boardHeight, boardPadding)
    {
        // populate the table
        for (int i = 0; i < gridWidth; i++)
        {
            for (int j = 0; j < gridHeight; j++)
            {
                if (i < padding || i >= gridWidth - padding || j < padding || j >= gridHeight - padding)
                {
                    update(i, j, 9);
                }
            }
        }
    }

    // Reset Method
    void resetGrid()
    {
        for (int i = 0; i < gridWidth; i++)
        {
            for (int j = 0; j < gridHeight; j++)
            {
                if (i < padding || i >= gridWidth - padding || j < padding || j >= gridHeight - padding)
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

    // Update Methods
    std::pair<int, int> boardToGrid(int x, int y)
    {
        // need to translate due to presence of padding for the walls
        return std::make_pair(x + padding, y + padding);
    }

    std::pair<int, int> boardToGrid(std::pair<int, int> coords)
    {
        // need to translate due to presence of padding for the walls
        return std::make_pair(coords.first + padding, coords.second + padding);
    }

    void updateWithPiece(piece targetPiece)
    {
        std::pair<int, int> gridCoords = boardToGrid(targetPiece.coords());
        int **pieceArray = targetPiece.array();

        int count = 0;

        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                if (pieceArray[j][i] != 0)
                {
                    update(gridCoords.first + i, gridCoords.second + j, pieceArray[j][i]);
                    count++;
                    if (count > 4)
                    {
                        return;
                    }
                }
            }
        }
    }

    // Move validity check
    bool checkMoveValidity(piece targetPiece)
    {
        // We make a copy of the grid and look only at a 6x6 area around the piece
        // This is to avoid checking the entire grid for every piece

        // translate board coordinates to grid coords
        std::pair newCoords = boardToGrid(targetPiece.coords());

        // Initalise a 6x6 grid
        int **gridCopy = new int *[6];
        for (int i = 0; i < 6; i++)
        {
            gridCopy[i] = new int[6];
            for (int j = 0; j < 6; j++)
            {
                if (this->gridArray[newCoords.first - 1 + i][newCoords.second - 1 + j] != 0)
                {
                    gridCopy[i][j] = 1;
                }
                else
                {
                    gridCopy[i][j] = 0;
                }
            }
        }

        int **pieceArray = targetPiece.array();

        // Place the piece in the new position into gridCopy
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                if (pieceArray[j][i] != 0)
                {
                    if (pieceArray[j][i] != 0)
                    {
                        gridCopy[i + 1][j + 1] += 1;
                    }

                    if (gridCopy[i + 1][j + 1] > 1)
                    {
                        return false;
                    }
                }
            }
        }

        return true;
    }

    // Line Clearance
    void clearLines(int *linesClearedp)
    {
        int count = 0;

        // Check through each line
        for (int j = gridHeight - padding - 1; j >= padding; j--)
        {
            for (int i = padding; i < gridWidth - padding; i++)
            {
                if (gridArray[i][j] != 0)
                {
                    count++;
                }
            }

            // hit the top of the all the blocks
            if (count == 0)
            {
                return;
            }

            // If the line is full, clear it
            if (count == gridWidth - 2 * padding)
            {
                *linesClearedp = *linesClearedp + 1;
                for (int i = padding; i < gridWidth - padding; i++)
                {
                    gridArray[i][j] = 0;
                }
            }

            count = 0;
        }
    }

    // Move Line Down
    void moveLineDown(int originLineNo, int noOfLines)
    {
        if (noOfLines == 0)
        {
            return;
        }

        for (int i = padding; i < gridWidth - padding; i++)
        {
            update(i, originLineNo + noOfLines, gridArray[i][originLineNo]);
            update(i, originLineNo, 0);
        }
    }

    bool checkRowIsZero(int row)
    {
        for (int i = padding; i < gridWidth - padding; i++)
        {
            if (gridArray[i][row] != 0)
            {
                return false;
            }
        }

        return true;
    }

    void moveAllLinesDown()
    {
        int emptyRows = 0;

        // We find the first free row in the grid
        for (int j = gridHeight - padding - 1; j >= padding; j--)
        {
            if (checkRowIsZero(j))
            {
                emptyRows++;
            }
            else
            {
                moveLineDown(j, emptyRows);
            }
        }
    }
};