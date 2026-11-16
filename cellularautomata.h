#pragma once

#include <array>

#define ALIVE 1
#define DEAD 0

class CellularAutomata {
public:
    static constexpr int GRID_WIDTH = 50;
    static constexpr int GRID_HEIGHT = 50;

    CellularAutomata();
    void setTile(int x, int y, int status); //sets the status of a tile at coordinate x,y
    int getAliveNeighbours(int x, int y);
    bool isAlive(int x, int y); //returns if a cell is alive or not (value of 1)
    void cycle();
    std::array<int, GRID_WIDTH * GRID_HEIGHT> get_grid() { return grid; }
    bool set_grid(std::array<int, GRID_WIDTH * GRID_HEIGHT> newGrid);
    int get_generations() { return generations; }
private:
    int generations{0};
    std::array<int, GRID_WIDTH * GRID_HEIGHT> grid{DEAD};
    std::array<int, GRID_WIDTH * GRID_HEIGHT> nextGrid{DEAD};

};