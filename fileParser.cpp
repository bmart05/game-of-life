#include "fileParser.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <sstream>
#include <string>

PatternData FileParser::openPattern(std::string filepath)
{
    std::ifstream file(filepath);
    if(!file.is_open()) {
        throw std::runtime_error("Failed to open pattern file: " + filepath + "\n");
    }

    PatternData data;

    std::string line;
    //extract all metadata from file
    while(std::getline(file, line)) {
        //check if line starts with #
        size_t start = line.find_first_not_of('\t\r\n');
        if(start == std::string::npos) { //if line is blank
            continue;
        }
        if(line[start] == '#') { //if line is a header
            char tag = std::toupper(static_cast<unsigned char>(line[start+1])); //get header tag
            std::string headerInfo = line.substr(start + 2);

            if(tag == 'N') { //pattern name
                data.metadata.name = headerInfo;
            }
            else if (tag == 'O' || tag == 'A')
            {
                data.metadata.author = headerInfo;
            }
            else if (tag == 'C')
            {
                data.metadata.comments += headerInfo + '\n';
            }
        }
        else { //must be x,y line
            //extract x and y
            std::istringstream headerStream(line);
            
            int x = 0, y = 0;
            std::string segment;

            while(std::getline(headerStream, segment, ',')) {
                size_t eqPos = segment.find('=');
                if(eqPos == std::string::npos) { continue; }

                std::istringstream keyStream(segment.substr(0,eqPos)); //key name (x or y)
                std::istringstream valStream(segment.substr(eqPos+1)); //key value

                std::string key;
                keyStream >> key;

                if(key == "x") {
                    valStream >> x;
                }
                else if (key == "y")
                {
                    valStream >> y;
                } 
            }

            data.width = x;
            data.height = y;


            break;
        }

    }

    //extract pattern data
    while(std::getline(file,line)) {
        size_t end = line.find_last_not_of("\t\r\n");
        line = line.erase(end+1);

        data.contents += line;
    }

    return data;
}

std::vector<int> FileParser::generateGrid(std::string pattern, int width, int height)
{  
    std::vector<int> grid(width * height, 0); //fill grid with dead cells

    std::istringstream patternStream(pattern);

    char ch;
    int count = 0; //stores current count for

    size_t x = 0;
    size_t y = 0;

    while(patternStream >> ch) {
        int run = (count == 0) ? 1 : count;

        if(std::isdigit(ch)) {
            count = count * 10 + (ch - '0'); //pushes previous digit back by 10, and adds on the new digit
        }
        else {
            if(ch == '$') {
                x = 0; //reset to start of line
                y += run; //advance y
            }
            else if(ch == '!') {
                break; //end of pattern
            }
            else if(ch == 'b') {
                    x += run;
            }
            else if(ch == 'o') {
                for(int i = 0; i < run; i++) {
                    if((x+i) >= width || y >= height) { break; }

                    grid[y*width + (x+i)] = 1;
                }
                x += run;

            }
            count = 0;
        }
    }

    return grid;
}

void FileParser::printPatternData(PatternData pattern)
{
    std::cout << "Name: " << pattern.metadata.name << "\n";
    std::cout << "Author: " << pattern.metadata.author << "\n";
    std::cout << "Comments: \n" << pattern.metadata.comments << "\n";
    std::cout << "Width: " << +pattern.width << " Height: " << +pattern.height << "\n"; //adding + before width and height promotes them to an integer
    std::cout << "Data: " << pattern.contents << "\n";
}
