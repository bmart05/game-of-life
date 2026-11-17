#include "fileParser.h"
#include <fstream>
#include <iostream>
#include <vector>

PatternData FileParser::openPattern(std::string filepath)
{
    std::ifstream file(filepath);
    if(!file.is_open()) {
        throw std::runtime_error("Failed to open pattern file: " + filepath + "\n");
    }

    PatternData data;

    std::string line;
    while(std::getline(file, line)) {
        //check if line starts with #
        //find line tag (N, O, C), if unknown ignore
        
        //extract x, y otherwise
        //then break

        std::cout << line << "\n";
    }



    return data;
}

std::vector<int> FileParser::generateGrid(std::string contents)
{
    return std::vector<int>();
}
