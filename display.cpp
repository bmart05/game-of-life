#include <SDL3/SDL.h>
#include <vector>
#include <array>
#include <format>
#include <iostream>
#include "display.h"

struct Display::Impl {
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    SDL_Texture* renderFrame = nullptr;

    int window_width;
    int window_height;

    int tex_width;
    int tex_height;
    std::vector<uint32_t> pixelBuffer;
};

Display::Display() : impl(std::make_unique<Impl>()) {}

Display::~Display() = default;

bool Display::init(const std::string title, int window_width, int window_height, int tex_width, int tex_height)
{
    if(!SDL_Init(SDL_INIT_VIDEO)) {
        std::cerr << "SDL Init failed: " << SDL_GetError() << "\n";
        return false;
    }

    if(!SDL_CreateWindowAndRenderer(title.c_str(), window_width, window_height, 0, &impl->window, &impl->renderer)) {
        std::cerr << "SDL_CreateWindowAndRenderer failed: " << SDL_GetError() << "\n";
        SDL_Quit();
        return false;
    }

    impl->window_width = window_width;
    impl->window_height = window_height;
    impl->tex_width = tex_width;
    impl->tex_height = tex_height;
    impl->pixelBuffer.resize(tex_width * tex_height, 0xFF000000); //set all pixels to black

    impl->renderFrame = SDL_CreateTexture(
        impl->renderer, 
        SDL_PIXELFORMAT_ARGB8888, 
        SDL_TEXTUREACCESS_STREAMING, 
        tex_width, 
        tex_height
    );

    SDL_SetTextureScaleMode(impl->renderFrame, SDL_SCALEMODE_NEAREST);


    return true;
}

void Display::render(const int *grid, std::optional<DebugInfo> debug)
{
    for(int i = 0; i < ((impl->tex_width)*(impl->tex_height)); i++) {
        impl->pixelBuffer[i] = grid[i] ? 0xFFFFFFFF : 0xFF000000;
    }
    SDL_UpdateTexture(impl->renderFrame, nullptr, impl->pixelBuffer.data(), impl->tex_width * sizeof(uint32_t));

    SDL_RenderClear(impl->renderer);
    SDL_RenderTexture(impl->renderer, impl->renderFrame, nullptr, nullptr);

    float cell_w = static_cast<float>(impl->window_width) / impl->tex_width;
    float cell_h = static_cast<float>(impl->window_height) / impl->tex_height;


    //-- DRAW GRID LINES -- (thanks gemini)
    // Subtle dark gray for grid lines
    SDL_SetRenderDrawColor(impl->renderer, 40, 40, 40, 255);

    // Vertical lines
    for (int x = 0; x <= impl->window_width; ++x) {
        float px = x * cell_w;
        SDL_RenderLine(impl->renderer, px, 0.0f, px, static_cast<float>(impl->window_height));
    }

    // Horizontal lines
    for (int y = 0; y <= impl->window_height; ++y) {
        float py = y * cell_h;
        SDL_RenderLine(impl->renderer, 0.0f, py, static_cast<float>(impl->window_width), py);
    }


    //draw debug text
    if(debug) {
        char buf[32];
        auto res = std::format_to_n(buf, sizeof(buf) - 1, "Generation: {} Paused: {}", debug->generations, debug->paused);
        *res.out = '\0';

        SDL_SetRenderDrawColor(impl->renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
        SDL_RenderDebugText(impl->renderer, 0, 0, buf);
    }

    SDL_RenderPresent(impl->renderer);
}

bool Display::handleInput()
{
    SDL_Event event;
    while(SDL_PollEvent(&event)) {
        switch(event.type) {
            case SDL_EVENT_QUIT:
                return false;
        }
    }
    return true;
}

void Display::shutdown()
{
    SDL_DestroyTexture(impl->renderFrame);
    SDL_DestroyRenderer(impl->renderer);
    SDL_DestroyWindow(impl->window);
    SDL_Quit();
}
