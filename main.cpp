#include "lastool.hpp"

// ./las-tools  '/media/sunj/0ea24735-792d-4934-9ab5-7fdfc57822f6/las-tools/test/Gridization'  '/media/sunj/0ea24735-792d-4934-9ab5-7fdfc57822f6/las-tools/test/Subsampling' Subsampling

int main (int argc, char * argv[])
{   
    std::string input = argv[1];
    std::string output = argv[2];
    std::string function = argv[3];

    LAS_TOOL lastool(input, output, function);
    
    return 1;
}    