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

    const std::array<SDL_Color, 19> colours = {{
        {  0,   0,   0, 255}, //  0: Dead (Black)

        // Reds -> Oranges
        {255,  45,  45, 255}, //  1: Crimson Red (Newborn)
        {255,  85,  35, 255}, //  2: Red-Orange
        {255, 125,  25, 255}, //  3: Deep Orange
        {255, 165,  20, 255}, //  4: Warm Orange

        // Yellows -> Limes
        {255, 205,  25, 255}, //  5: Amber / Gold
        {245, 235,  35, 255}, //  6: Bright Yellow
        {195, 240,  45, 255}, //  7: Yellow-Lime
        {135, 235,  65, 255}, //  8: Lime Green

        // Greens -> Teals
        { 70, 225,  95, 255}, //  9: Emerald Green
        { 40, 215, 145, 255}, // 10: Seafoam / Mint
        { 30, 205, 195, 255}, // 11: Turquoise / Teal

        // Cyans -> Blues
        { 30, 185, 240, 255}, // 12: Bright Cyan
        { 40, 145, 255, 255}, // 13: Sky Blue
        { 55, 105, 250, 255}, // 14: Dodger Blue
        { 75,  70, 240, 255}, // 15: Cobalt Blue

        // Indigos -> Violets
        {105,  55, 230, 255}, // 16: Indigo
        {135,  50, 215, 255}, // 17: Royal Purple
        {160,  45, 190, 255}  // 18: Deep Violet (Old / Stable)
    }};
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
        int age = std::min(grid[i], 18);
        SDL_Color cellColour = impl->colours[age];

        impl->pixelBuffer[i] = (static_cast<Uint32>(cellColour.a) << 24) |
                               (static_cast<Uint32>(cellColour.r) << 16) |
                               (static_cast<Uint32>(cellColour.g) << 8)  |
                               (static_cast<Uint32>(cellColour.b));
    }
    SDL_UpdateTexture(impl->renderFrame, nullptr, impl->pixelBuffer.data(), impl->tex_width * sizeof(uint32_t));

    SDL_RenderClear(impl->renderer);
    SDL_RenderTexture(impl->renderer, impl->renderFrame, nullptr, nullptr);

    float cell_w = static_cast<float>(impl->window_width) / impl->tex_width;
    float cell_h = static_cast<float>(impl->window_height) / impl->tex_height;


    //-- DRAW GRID LINES -- (thanks gemini)
    if(renderGridLines) {
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
}


    //draw debug text
    if(debug) {
        char buf[256];
        auto res = std::format_to_n(buf, sizeof(buf) - 1, "Name: {} Author: {}", debug->metadata.name, debug->metadata.author);
        *res.out = '\0';

        SDL_SetRenderDrawColor(impl->renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
        SDL_RenderDebugText(impl->renderer, 0, 0, buf);

        res = std::format_to_n(buf, sizeof(buf) - 1, "Generation: {} Paused: {}", debug->generations, debug->paused);
        *res.out = '\0';

        SDL_SetRenderDrawColor(impl->renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
        SDL_RenderDebugText(impl->renderer, 0, 10, buf);

        res = std::format_to_n(buf, sizeof(buf) - 1, "Speed: {}", *debug->targetStep);
        *res.out = '\0';

        SDL_SetRenderDrawColor(impl->renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
        SDL_RenderDebugText(impl->renderer, 0, 20, buf);
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
