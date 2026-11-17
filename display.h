#pragma once

#include <string>
#include <memory>
#include <optional>
#include <chrono>
#include "fileParser.h"

struct DebugInfo {
    int generations;
    bool paused;
    std::chrono::milliseconds* targetStep;
    PatternMetadata metadata;
};

class Display {
public:
    bool renderGridLines = true;

    Display();
    ~Display();

    Display(const Display&) = delete;
    Display& operator=(const Display&) = delete;

    bool init(const std::string title, int window_width, int window_height, int tex_width, int tex_height);
    void render(const int* grid, std::optional<DebugInfo> debug = std::nullopt);
    bool handleInput();
    void shutdown();
private:
    struct Impl;
    std::unique_ptr<Impl> impl;
};