#include <iostream>
#include <complex>
#include "piece.hpp"

class piece
{
    private:

        int shapeNumber;
        int orientation;
        std::pair<int,int> coordinates;
        int **pieceArray;

    public:
            
            piece(int shapeNumber = 0, int orientation = 0, int x_pos = 0, int y_pos = 0)
            {
                this->shapeNumber = shapeNumber;
                this->orientation = orientation;
                this->coordinates.first = x_pos;
                this->coordinates.second = y_pos;

                this->pieceArray = new int*[4];
                for(int i=0;i<4;i++)
                {
                    this->pieceArray[i] = new int[4];
                    for(int j=0;j<4;j++)
                    {
                        this->pieceArray[i][j] = pieceData[shapeNumber][orientation][i][j];
                    }
                }

            }
    
            void print()
            {
                std::cout << "Shape number: " << shapeNumber << std::endl;
                std::cout << "Orientation: " << orientation << std::endl;
                std::cout << "Coordinates: " << coordinates.first << " , " << coordinates.second << std::endl;

                for(int i=0; i<4; i++)
                {
                    for(int j=0; j<4; j++)
                    {
                        std::cout << pieceArray[i][j];
                    }
                    std::cout << std::endl;
                }

                std::cout << std::endl;
            }

            std::pair<int,int> coords()
            {
                return this->coordinates;
            }

            int** array()
            {
                return this->pieceArray;
            }

            void updateCoords(std::pair<int,int> newCoords)
            {
                this->coordinates = newCoords;
            }
};