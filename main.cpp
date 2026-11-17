#include "cellularautomata.h"
#include "display.h"
#include "fileParser.h"
#include <SDL3/SDL.h>
#include <chrono>
#include <iostream>
#include <string>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

int main(int argc, char **argv) {
    if(argc < 1) {
        return 0;
    }
    
    PatternData pattern = FileParser::openPattern(argv[1]);
    FileParser::printPatternData(pattern);

    
    Display display;
    CellularAutomata grid(pattern.width, pattern.height);
    
    //Disable grid lines if pattern size is bigger than half the window
    //There needs to be enough space to actually render them, and they take half the screen
    if(pattern.width >= (WINDOW_WIDTH/2) || pattern.height >= (WINDOW_HEIGHT/2)) {
        std::cout << "Disabling grid lines" << "\n";
        display.renderGridLines = false;
    }

    auto patternGrid = FileParser::generateGrid(pattern.contents, pattern.width, pattern.height);
    grid.set_grid(patternGrid);

    display.init("Game of Life", WINDOW_WIDTH, WINDOW_HEIGHT, grid.gridWidth, grid.gridHeight);


    auto targetStep = std::chrono::milliseconds(250);
    constexpr auto TARGET_STEP_CHANGE = std::chrono::milliseconds(50); //amount to change target step by when changing speed
    auto last_time = std::chrono::steady_clock::now();
    auto accumulator = std::chrono::steady_clock::duration{0};

    DebugInfo debugInfo = {
        0, true, &targetStep
    };

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
                        case SDLK_R:
                            grid.reset_grid();
                            break;
                        case SDLK_UP:
                            targetStep = targetStep + TARGET_STEP_CHANGE;
                            break;
                        case SDLK_DOWN:
                            targetStep = std::max(targetStep - TARGET_STEP_CHANGE, std::chrono::milliseconds(TARGET_STEP_CHANGE)); //don't let target step get less than zero
                            break;
                    }
                    break;
                case SDL_EVENT_MOUSE_BUTTON_DOWN:
                    if(event.button.button == SDL_BUTTON_LEFT) {
                        float cell_w = 800.0f / grid.gridWidth;
                        float cell_h = 600.0f / grid.gridHeight;

                        int col = static_cast<int>(event.button.x / cell_w);
                        int row = static_cast<int>(event.button.y / cell_h);

                        if (col >= 0 && col < grid.gridWidth && row >= 0 && row < grid.gridHeight) {
                            int status = grid.get_grid()[row*grid.gridHeight + col];
                            grid.setTile(col, row, !status);
                        }
                    } 
            }
        }
        debugInfo.generations = grid.get_generations();

        if(!paused) {
            accumulator += dt;
            if(accumulator >= targetStep) {
                grid.cycle();
                accumulator -= targetStep;
            }
        }


        display.render(grid.get_grid().data(), debugInfo);

        
    }
    display.shutdown();

    return 0;
}