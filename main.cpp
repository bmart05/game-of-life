#include "cellularautomata.h"
#include "display.h"
#include <SDL3/SDL.h>
#include <chrono>
#include <iostream>

int main() {
    CellularAutomata grid;
    Display display;
    DebugInfo debugInfo = {
        0, true
    };

    display.init("Game of Life", 800, 600, grid.GRID_WIDTH, grid.GRID_HEIGHT);


    constexpr auto TARGET_STEP = std::chrono::milliseconds(250);
    auto last_time = std::chrono::steady_clock::now();
    auto accumulator = std::chrono::steady_clock::duration{0};

    bool running = true;
    bool paused = true;

    while(running) {
        auto current_time = std::chrono::steady_clock::now();
        auto dt = current_time - last_time;
        last_time = current_time;

        SDL_Event event;
        while(SDL_PollEvent(&event)) {
            switch(event.type) {
                case SDL_EVENT_QUIT:
                    running = false;
                    break;
                case SDL_EVENT_KEY_DOWN:
                    switch(event.key.key) {
                        case SDLK_SPACE:
                            paused = !paused;
                            debugInfo.paused = !debugInfo.paused;
                            break;
                        case SDLK_C:
                            grid.cycle();
                            break;
                    }
                    break;
                case SDL_EVENT_MOUSE_BUTTON_DOWN:
                    if(event.button.button == SDL_BUTTON_LEFT) {
                        constexpr float cell_w = 800.0f / grid.GRID_WIDTH;
                        constexpr float cell_h = 600.0f / grid.GRID_HEIGHT;

                        int col = static_cast<int>(event.button.x / cell_w);
                        int row = static_cast<int>(event.button.y / cell_h);

                        if (col >= 0 && col < grid.GRID_WIDTH && row >= 0 && row < grid.GRID_HEIGHT) {
                            int status = grid.get_grid()[row*grid.GRID_HEIGHT + col];
                            grid.setTile(col, row, !status);
                        }
                    } 
            }
        }
        debugInfo.generations = grid.get_generations();

        if(!paused) {
            accumulator += dt;
            if(accumulator >= TARGET_STEP) {
                grid.cycle();
                accumulator -= TARGET_STEP;
            }
        }


        display.render(grid.get_grid().data(), debugInfo);

        
    }
    display.shutdown();

    return 0;
}