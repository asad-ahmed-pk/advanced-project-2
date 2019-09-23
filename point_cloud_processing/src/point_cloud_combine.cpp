// point_cloud_combine.cpp
// Combines fumarole point clouds in the directory into 1 point cloud

#include <iostream>
#include <string>
#include <vector>

#include <pcl/point_types.h>
#include <pcl/io/pcd_io.h>

#include <boost/filesystem.hpp>

void GetPCDFilesInDir(const std::string& dir, std::vector<boost::filesystem::directory_entry>& filePaths);

int main(int argc, char** argv)
{
    // read directory as cmd argument
    if (argc < 2) {
        std::cout << "\nUsage: point_cloud_combine [directory_path]" << std::endl;
        return 1;
    }

    std::string dir { argv[1] };
    std::cout << "Checking files in directory: " << dir << std::endl;
    std::vector<boost::filesystem::directory_entry> pointCloudFilePaths;

    GetPCDFilesInDir(dir, pointCloudFilePaths);

    // print out number of pcd files found
    std::cout << "Found " << pointCloudFilePaths.size() << " entries" << std::endl;

    return 0;
}

// Stores all file paths for PCD files in the given directory
void GetPCDFilesInDir(const std::string& dir, std::vector<boost::filesystem::directory_entry>& filePaths)
{
    // get list of files inside directory
    boost::filesystem::directory_iterator it;
    for (auto filePath : boost::filesystem::directory_iterator(dir))
    {
        if (boost::filesystem::is_regular_file(filePath))
        {
            if (filePath.path().extension().string() == ".pcd") {
                filePaths.push_back(filePath);
            }
        }
    }
}
