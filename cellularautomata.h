#pragma once

#include <array>
#include <vector>

#define ALIVE 1
#define DEAD 0

class CellularAutomata {
public:
    int gridWidth = 50;
    int gridHeight = 50;

    static constexpr int MAX_CELLAGE = 18;

    CellularAutomata(int width, int height);
    void setTile(int x, int y, int status); //sets the status of a tile at coordinate x,y
    int getAliveNeighbours(int x, int y); //returns all alive tiles neighbouring x,y
    bool isAlive(int x, int y); //returns if a cell is alive or not (value of 1)
    void cycle(); //complete one cycle of the automata
    std::vector<int> get_grid() { return grid; }
    bool set_grid(std::vector<int> newGrid); //sets entire value of grid
    int get_generations() { return generations; }
    void reset_grid(); //zeroes out grid, removing all alive tiles
private:
    int generations{0};
    //std::array<int, GRID_WIDTH * GRID_HEIGHT> grid{DEAD};
    std::vector<int> grid;
    std::vector<int> nextGrid{DEAD};

};