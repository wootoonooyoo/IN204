#include <iostream>
#include <complex>
#include "piece.hpp"

class piece
{
    private:
        int shapeNumber;
        int orientation;
        int previousOrientation;
        std::pair<int,int> coordinates;
        std::pair<int,int> previousCoordinates;
        int **pieceArray;

    public:
            piece(int shapeNumber = 0, int orientation = 0, int x_pos = 3, int y_pos = 0)
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
    
            // Print Methods
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

            void printCoords()
            {
                std::cout << "Coordinates: " << this->coordinates.first << " , " << this->coordinates.second << std::endl;
            }
            
            // Return Methods
            std::pair<int,int> coords()
            {
                return this->coordinates;
            }

            int** array()
            {
                return this->pieceArray;
            }

            // Rotate Method
            void rotate()
            {
                previousOrientation = this->orientation;
                this->orientation = (this->orientation + 1) % 4;

                for(int i=0;i<4;i++)
                {
                    this->pieceArray[i] = new int[4];
                    for(int j=0;j<4;j++)
                    {
                        this->pieceArray[i][j] = pieceData[shapeNumber][orientation][i][j];
                    }
                }
                
            }

            // Update Methods
            void updateCoords(std::pair<int,int> newCoords)
            {
                this->coordinates = newCoords;
            }

            void updateCoords(int xpos, int ypos)
            {
                this->coordinates.first = xpos;
                this->coordinates.second = ypos;
            }

            void move(int x_change, int y_change)
            {
                this->previousCoordinates = this->coordinates;
                this->coordinates.first += x_change;
                this->coordinates.second += y_change;
            }

            void revert()
            {
                this->coordinates = this->previousCoordinates;
            }

            void revertRotation()
            {
                this->orientation = this->previousOrientation;

                for(int i=0;i<4;i++)
                {
                    this->pieceArray[i] = new int[4];
                    for(int j=0;j<4;j++)
                    {
                        this->pieceArray[i][j] = pieceData[shapeNumber][orientation][i][j];
                    }
                }
            }


};