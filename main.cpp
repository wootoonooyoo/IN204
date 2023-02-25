#include <iostream>
#include <array>
#include "grid.cpp"
#include "piece.cpp"
#include "piece.hpp"
#include "window.cpp"

int main()
{
    // Create a grid then load a piece in then print the grid
    gameGrid baseGrid(10,20,2);

    piece piece1(3,2,3,0);
    gameWindow w(100, 100, 800, 800);
    std::pair<int,int> coords{3,0};
    
    piece1.updateCoords(coords);
    baseGrid.updateWithPiece(piece1.coords(),piece1.array());
    w.renderGrid(baseGrid.array());
    w.drawGrid(baseGrid.size());
    w.render();

    w.clearBuffer();
    coords.second += 1;
    // copy contents of baseGrid to stagingGrid
    //stagingGrid.updateArray(baseGrid.array());
    baseGrid.print();



}