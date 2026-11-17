#include "cellularautomata.h"

CellularAutomata::CellularAutomata(int width, int height)
{
    gridWidth = width;
    gridHeight = height;

    grid.assign(width * height, DEAD); //initialise grid with dead cells
    nextGrid.assign(width * height, DEAD);
}

void CellularAutomata::setTile(int x, int y, int status)
{
    if(x < 0 || x >= gridWidth) { return; }
    if(y < 0 || y >= gridHeight) { return; }

    grid[y * gridWidth + x] = status; //row-major order (top to bottom)
}

int CellularAutomata::getAliveNeighbours(int x, int y)
{
    int alive = 0;
    for(int i = x-1; i <= x+1; i++) {
        if(i < 0 || i > gridWidth) { continue; }

        for(int j = y-1; j <= y+1; j++) {
            if(j < 0 || j > gridHeight) { continue; }

            if(i == x && j == y) { continue; }
            
            if(isAlive(i,j)) { 
                alive++;
            }
        }
    }   
    return alive;
}

bool CellularAutomata::isAlive(int x, int y)
{
    if(x < 0 || x >= gridWidth) { return false; }
    if(y < 0 || y >= gridHeight) { return false; }

    int gridPosition = y * gridWidth + x;
    if(grid[gridPosition] >= 1) { 
        return true;
    }
    return false;
}

void CellularAutomata::cycle()
{
    for(int y = 0; y < gridHeight; y++) {
        for(int x = 0; x < gridWidth; x++) {
            int aliveNeighbours = getAliveNeighbours(x, y);
            int targetCell = y * gridWidth + x;

            if(isAlive(x,y)) {
                if(aliveNeighbours < 2 || aliveNeighbours > 3) {
                    nextGrid[targetCell] = DEAD;
                }
                else {
                    nextGrid[targetCell] = std::min(MAX_CELLAGE, nextGrid[targetCell] + 1); //increase age of cell every cycle (up to the max age)
                }
            }
            else {
                if(aliveNeighbours == 3) {
                    nextGrid[targetCell] = ALIVE;
                }
                else {
                    nextGrid[targetCell] = DEAD;
                } 
            }
        }
    }
    std::swap(grid, nextGrid);
    generations++;
}

bool CellularAutomata::set_grid(std::vector<int> newGrid)
{
    if(newGrid.size() != grid.size()) { return false; }

    std::swap(grid, newGrid);
    return true;
}

void CellularAutomata::reset_grid()
{
    generations = 0;
    std::fill(grid.begin(), grid.end(), DEAD);
}
