#ifndef UTILS_HPP
#define UTILS_HPP
#define PCL_NO_PRECOMPILE

// STL;
#include <string>
#include <vector>
#include <iostream>

// boost library;
#include <boost/filesystem.hpp>
#include <boost/format.hpp>
#include <boost/tokenizer.hpp>

// pcl library
#include <pcl/io/pcd_io.h>
#include <pcl/point_types.h>
#include <pcl/point_cloud.h>

namespace pcl
{
    struct PointXYZRGBI
    {
        PCL_ADD_POINT4D;
        PCL_ADD_RGB;
        PCL_ADD_INTENSITY;

        EIGEN_MAKE_ALIGNED_OPERATOR_NEW
    } EIGEN_ALIGN16;
}
POINT_CLOUD_REGISTER_POINT_STRUCT(PointXYZRGBI, (float, x, x)(float, y, y)(float, z, z)(float, rgb, rgb)(float, intensity, intensity));

// Eigen library;
#include <eigen3/Eigen/Dense>
#include <eigen3/Eigen/Geometry>

// liblas library;
#include <liblas/liblas.hpp>

// opencv libaray;
#include <opencv2/opencv.hpp>

using Eigen4x4d = Eigen::Matrix<double, 4, 4>;
using Eigen4x1d = Eigen::Matrix<double, 4, 1>;
using Eigen3x4d = Eigen::Matrix<double, 3, 4>;
using Eigen3x3d = Eigen::Matrix<double, 3, 3>;
using Eigen3x1d = Eigen::Matrix<double, 3, 1>;
using Eigen2x1d = Eigen::Matrix<double, 2, 1>;

std::vector<std::string> listdir(const std::string &directoryPath, const std::string &extension = "");
std::vector<std::string> tokenize(std::string context, const std::string &delim = " ");

void pcl2las(const std::string &lasPath, pcl::PointCloud<pcl::PointXYZRGBI>::Ptr cloud, const double &shift_x, const double &shift_y, const double &shift_z);
void las2pcl(const std::string &lasPath, pcl::PointCloud<pcl::PointXYZRGBI>::Ptr &cloud, double &shift_x, double &shift_y, double &shift_z, bool isOrigin = false);

std::string FileInformation(const std::string &filePath, const std::string &information);

template <typename N>
std::string zeroPadding(const N &number, const int numberOfZeros)
{
    std::string count_str = std::to_string(number);
    std::string padded_str = std::string(numberOfZeros - count_str.length(), '0') + count_str;

    return padded_str;
}

template<typename T, typename U>
bool all_equal(T&& t, U&& u)
{
    return (t == u);
}

template<typename T, typename U, typename... Ts>
bool all_equal(T&& t, U&& u, Ts&&... args)
{
    return (t == u) && all_equal(u, std::forward<Ts>(args)...);
}

constexpr unsigned int Hashcode(const char* str)
{
    return str[0] ? static_cast<unsigned int>(str[0]) + 0xEDB8832Full * Hashcode(str + 1) : 8603;
}

#endif