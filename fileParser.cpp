#include "fileParser.h"
#include <fstream>
#include <iostream>

PatternData FileParser::openPattern(std::string filepath)
{
    std::ifstream file(filepath);
    if(!file.is_open()) {
        throw std::runtime_error("Failed to open pattern file: " + filepath + "\n");
    }

    std::string line;
    while(std::getline(file, line)) {
        std::cout << line << "\n";
    }
}

std::vector<int> FileParser::generateGrid(std::string contents)
{
    return std::vector<int>();
}
