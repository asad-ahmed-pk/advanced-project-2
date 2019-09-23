# Advanced Project II

cmake_minimum_required(VERSION 3.10)
project(advanced_project_2)

set(CMAKE_CXX_STANDARD 14)

# PCL
find_package(PCL 1.8 REQUIRED COMPONENTS common io)
include_directories(${PCL_INCLUDE_DIRS})
link_directories(${PCL_LIBRARY_DIRS})
add_definitions(${PCL_DEFINITIONS})

# Eigen
find_package (Eigen3 3.3 REQUIRED)
include_directories($ENV{EIGEN3_INCLUDE_DIR})

# Boost
find_package(Boost REQUIRED COMPONENTS filesystem)
include_directories(${Boost_INCLUDE_DIR})

## ----- Executables -----

# Point cloud combiner
add_executable(point_cloud_combine src/point_cloud_combine.cpp)
target_link_libraries(point_cloud_combine ${PCL_LIBRARIES} ${Boost_LIBRARIES})
