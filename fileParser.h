#pragma once
#include <string>
#include <vector>
#include <cstdint>

struct PatternMetadata {
    std::string name;
    std::string author;
    std::vector<std::string> comments;
};

struct PatternData {
    PatternMetadata metadata;
    uint8_t width;
    uint8_t height;
    std::string contents;
};

class FileParser {
    static PatternData openPattern(std::string filepath);
    static std::vector<int> generateGrid(std::string contents);
};
