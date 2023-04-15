#include "lastool.hpp"

int main (int argc, char * argv[])
{   
    std::string input = argv[1];
    std::string output = argv[2];
    std::string function = argv[3];

    LAS_TOOL lastool(input, output, function);
    
    return 1;
}    