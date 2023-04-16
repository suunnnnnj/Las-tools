cmake_minimum_required(VERSION 3.2)
set(CMAKE_BUILD_TYPE Release)
add_compile_options(-std=c++17)

project(las-tools)

find_package(PCL REQUIRED)
find_package(libLAS REQUIRED)
find_package(OpenCV REQUIRED)
find_package(Boost COMPONENTS filesystem REQUIRED)
find_package(OpenMP)

if (OPENMP_FOUND)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()

include_directories(include include/utils ${PCL_INCLUDE_DIRS} ${OpenCV_LIBS})
link_directories(${PCL_LIBRARY_DIRS})
add_definitions(${PCL_DEFINITIONS})

add_executable (las-tools main.cpp src/lastool.cpp src/utils.cpp)
target_link_libraries (las-tools ${OpenCV_LIBS} ${PCL_LIBRARIES} ${libLAS_LIBRARIES})
