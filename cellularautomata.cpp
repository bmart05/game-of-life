#include "cellularautomata.h"

CellularAutomata::CellularAutomata()
{
}

void CellularAutomata::setTile(int x, int y, int status)
{
    if(x < 0 || x >= GRID_WIDTH) { return; }
    if(y < 0 || y >= GRID_HEIGHT) { return; }

    grid[y * GRID_WIDTH + x] = status; //row-major order (top to bottom)
}

int CellularAutomata::getAliveNeighbours(int x, int y)
{
    int alive = 0;
    for(int i = x-1; i <= x+1; i++) {
        if(i < 0 || i > GRID_WIDTH) { continue; }

        for(int j = y-1; j <= y+1; j++) {
            if(j < 0 || j > GRID_HEIGHT) { continue; }

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
    if(x < 0 || x >= GRID_WIDTH) { return false; }
    if(y < 0 || y >= GRID_HEIGHT) { return false; }

    int gridPosition = y * GRID_WIDTH + x;
    if(grid[gridPosition] >= 1) { 
        return true;
    }
    return false;
}

void CellularAutomata::cycle()
{
    for(int y = 0; y < GRID_HEIGHT; y++) {
        for(int x = 0; x < GRID_WIDTH; x++) {
            int aliveNeighbours = getAliveNeighbours(x, y);
            int targetCell = y * GRID_WIDTH + x;

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

bool CellularAutomata::set_grid(std::array<int, GRID_WIDTH * GRID_HEIGHT> newGrid)
{
    if(newGrid.size() != grid.size()) { return false; }

    std::swap(grid, newGrid);
    return true;
}

void CellularAutomata::reset_grid()
{
    grid.fill(DEAD);
}
