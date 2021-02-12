//
// Created by konstantin on 03/02/2021.
//

#include <string>
#include <fstream>
#include <sstream>

std::string readFileContents(std::string path) {
    std::ifstream in(path);
    std::stringstream buffer;
    buffer << in.rdbuf();
    return buffer.str();
}
