#pragma once
#include <string>
#include <vector>
#include <cstdint>

struct PatternMetadata {
    std::string name;
    std::string author;
    std::string comments; //usually multiple lines, will be split with \n
};

struct PatternData {
    PatternMetadata metadata;
    uint8_t width;
    uint8_t height;
    std::string contents;
};

class FileParser {
public:
    static PatternData openPattern(std::string filepath);
    static std::vector<int> generateGrid(std::string pattern, int width, int height);
    static void printPatternData(PatternData pattern);
};
 