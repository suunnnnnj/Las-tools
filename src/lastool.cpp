#include "lastool.hpp"

void LAS_TOOL::Gridization()
{
    double length;
    
    std::cout << "Enter the length of grid : ";
    std::cin >> length;

    tqdm bar;
    
    std::unordered_map<std::string, std::vector<liblas::Point>> pointcloudGroup;

    bool setShift = false;

    std::cout << "-------------------------" << std::endl;
    std::cout << "las Gridizaion..." << std::endl;

    for(int lIdx = 0 ; lIdx < laslist_.size(); lIdx++)
    {
        bar.progress(lIdx, laslist_.size());

        std::ifstream ifs;
        ifs.open(laslist_[lIdx], std::ios::in | std::ios::binary);

        if(!ifs.is_open())
        {
            std::cout << "Error opening input : " << laslist_[lIdx] << std::endl;
            exit(-1);
        }

        liblas::ReaderFactory f ;
        liblas::Reader reader = f.CreateWithStream(ifs);

        while (reader.ReadNextPoint())
        {

            if(!setShift)
            {
                liblas::Point const &p = reader.GetPoint();

                shift_x_ = p.GetX();
                shift_y_ = p.GetY();
                shift_z_ = p.GetZ(); 
                
                setShift = true;
            }

            if (IsKITTIDataset)
            {
                liblas::Point const &p = reader.GetPoint();

                double x = p.GetX();
                double z = p.GetZ();

                int xIndex = floor(x / length);
                int zIndex = floor(z / length);

                std::string key = std::to_string(xIndex) + "_" + std::to_string(zIndex);
                pointcloudGroup[key].push_back(p);
            }
            else
            {
                liblas::Point const &p = reader.GetPoint();

                double x = p.GetX();
                double y = p.GetY();

                int xIndex = floor(x / length);
                int yIndex = floor(y / length);

                std::string key = std::to_string(xIndex) + "_" + std::to_string(yIndex);
                pointcloudGroup[key].push_back(p);
            }
        }

        ifs.close();
    }

    std::cout << '\n' << "-------------------------" << std::endl;
    std::cout << "Exporting Gridizated las.." << std::endl;

    
    liblas::Header header;
    header.SetDataFormatId(liblas::ePointFormat3);
    // header.SetOffset(shift_x_, shift_y_, shift_z_);
    header.SetScale(0.01, 0.01, 0.01);
    
    int lasIdx = 0;

    for(auto & [key, pointcloud] : pointcloudGroup)
    {
        bar.progress(lasIdx, pointcloudGroup.size());
        
        std::string lasPath = outputDir_ + "/" + key + ".las";
        std::ofstream ofs(lasPath, std::ios::out | std::ios::binary);
        
        liblas::Writer writer(ofs, header);

        for(liblas::Point & point : pointcloud)
            writer.WritePoint(point);
        
        lasIdx++;
    }

    std::cout <<  "-------------------------" << std::endl;

    return;
}

void LAS_TOOL::Subsampling()
{
    double radius;
    
    std::cout << "Enter the radius(cm) : ";
    std::cin >> radius;
    
    tqdm bar;
    bool isOrigin = false; 
    
    for(int lIdx = 0 ; lIdx < laslist_.size(); lIdx++)
    {
        bar.progress(lIdx, laslist_.size());

        pcl::PointCloud<pcl::PointXYZRGBI>::Ptr cloud(new pcl::PointCloud<pcl::PointXYZRGBI>);
        pcl::PointCloud<pcl::PointXYZRGBI>::Ptr subcloud(new pcl::PointCloud<pcl::PointXYZRGBI>);

        las2pcl(laslist_[lIdx], cloud, shift_x_, shift_y_, shift_z_, isOrigin);

        pcl::UniformSampling<pcl::PointXYZRGBI> filter;
        filter.setInputCloud(cloud); 
        filter.setRadiusSearch(radius/100.0);
        filter.filter(*subcloud); 

        std::string lasPath;
        lasPath = outputDir_ + "/" + FileInformation(laslist_[lIdx], "filename");
        pcl2las(lasPath, subcloud, shift_x_, shift_y_, shift_z_);
    }

    return;
}

void LAS_TOOL::Denoising()
{
    return;
}