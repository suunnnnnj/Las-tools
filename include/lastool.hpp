#ifndef LASTOOL_HPP
#define LASTOOL_HPP

#include "utils.hpp"
#include "tqdm.h"

class LAS_TOOL
{
    public:

        LAS_TOOL(const std::string &inputDir, const std::string &outputDir, const std::string &function){
            
            laslist_ = listdir(inputDir, ".las");

            outputDir_ = outputDir;
            
            if(boost::filesystem::exists(outputDir_))
                boost::filesystem::create_directories(outputDir_);

            switch( Hashcode(function.c_str()) )
            {
                
            case Hashcode("Gridization"):
                Gridization();
                break;
            
            case Hashcode("Subsampling"):
                Subsampling();
                break;

            case Hashcode("Denoising"):
                Denoising();
                break;

            }
        };

        ~LAS_TOOL(){

        };

    private:

        std::vector<std::string> laslist_;
        std::string outputDir_;

        void Gridization();
        void Subsampling();
        void Denoising();

        
};

#endif