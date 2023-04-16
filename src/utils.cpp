#include "utils.hpp"

std::vector<std::string> listdir(const std::string &directoryPath, const std::string &extension)
{
    std::vector<std::string> list;

    for(auto path : boost::filesystem::directory_iterator(directoryPath))
    {
        if(extension != "")
        {
            if(path.path().extension() == extension)
                list.push_back(path.path().string());
        }
        else
        {
            list.push_back(path.path().string());
        }
    }   

    sort(list.begin(), list.end());

    return list;
}

std::vector<std::string> tokenize(std::string context, const std::string &delim)
{
    std::vector<std::string> tokens;
 
    boost::char_separator<char> sep(delim.c_str());
    boost::tokenizer<boost::char_separator<char>> tokenizer(context, sep);
 
    for (const std::string &s: tokenizer) 
        tokens.push_back(s);

    return tokens;
}

void pcl2las(const std::string &lasPath, pcl::PointCloud<pcl::PointXYZRGBI>::Ptr cloud, const double &shift_x, const double &shift_y, const double &shift_z)
{
    std::ofstream ofs(lasPath, std::ios::out | std::ios::binary);
    
    liblas::Header header;
    header.SetDataFormatId(liblas::ePointFormat3);
    header.SetOffset(shift_x, shift_y, shift_z);
    header.SetScale(0.01, 0.01, 0.01);

    liblas::Writer writer(ofs, header);

    for (size_t i = 0; i < cloud->points.size(); ++i)
    {
        liblas::Point point(&header);
        double x = static_cast<double>(cloud->points[i].x) + shift_x;
        double y = static_cast<double>(cloud->points[i].y) + shift_y;
        double z = static_cast<double>(cloud->points[i].z) + shift_z;
        double intensity = static_cast<double>(cloud->points[i].intensity);

        point.SetX(x);
        point.SetY(y);
        point.SetZ(z);
        point.SetIntensity(intensity);

        int r1 = ceil((float)cloud->points[i].r);
        int g1 = ceil((float)cloud->points[i].g);
        int b1 = ceil((float)cloud->points[i].b);

        uint32_t r = static_cast<uint32_t>(r1);
        uint32_t g = static_cast<uint32_t>(g1);
        uint32_t b = static_cast<uint32_t>(b1);

        liblas::Color color(r, g, b);
        point.SetColor(color);
        writer.WritePoint(point);
    }
}

void las2pcl(const std::string &lasPath, pcl::PointCloud<pcl::PointXYZRGBI>::Ptr &cloud, double &shift_x, double &shift_y, double &shift_z, bool isOrigin)
{
    return;
}

std::string FileInformation(const std::string &filePath, const std::string &information)
{
    boost::filesystem::path p{filePath};

    std::string info;

    switch(Hashcode(information.c_str()))
    {
        case Hashcode("filename"):
            info = p.filename().string();
            break;
        
        case Hashcode("filename_stem"):
            info = p.filename().stem().string();
            break;
            
        case Hashcode("parent_path"):
            info = p.parent_path().string();
            break;
        
        case Hashcode("extension"):
            info = p.extension().string();
            break;

        case Hashcode("root_path"):
            info = p.root_path().string();
            break;
    }

    return info;
}