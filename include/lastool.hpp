#ifndef LASTOOL_HPP
#define LASTOOL_HPP

#include "utils.hpp"
#include "tqdm.h"

#include <iostream>
#include <pcl/io/pcd_io.h>
#include <pcl/point_types.h>
#include <pcl/filters/uniform_sampling.h>

class LAS_TOOL
{
    public:

        LAS_TOOL(const std::string &inputDir, const std::string &outputDir, const std::string &function){
            
            laslist_ = listdir(inputDir, ".las");

            outputDir_ = outputDir;

            if (boost::filesystem::exists(outputDir_))
                boost::filesystem::create_directories(outputDir_);

            std::string las0 = laslist_[0];
            std::ifstream ifs(las0, std::ios::in | std::ios::binary);
            
            if (!ifs.is_open())
            {
                std::cout << "Error opening input : " << las0 << std::endl;
                exit(-1);
            }

            liblas::ReaderFactory f;
            liblas::Reader reader = f.CreateWithStream(ifs);
            reader.ReadNextPoint();

            liblas::Point point = reader.GetPoint();
            
            shift_x_ = point.GetX();
            shift_y_ = point.GetY();
            shift_z_ = point.GetZ();

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

        double shift_x_, shift_y_, shift_z_;

        bool IsKITTIDataset = false;
};

#endif