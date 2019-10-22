//
// DatasetLoader.cpp
// Loads data from the file system
//

#include "io/DatasetLoader.hpp"
#include "config/config.hpp"

#include <iostream>
#include <fstream>

#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

namespace IO
{
    // Load test file IDs
    void DatasetLoader::GetTestFiles(std::map<std::string, std::string>& files)
    {
        std::ifstream is;
        is.open(Config::TEST_ID_LIST_PATH, std::ios::in);

        std::string fileID;
        std::string filePath;

        if (is.is_open())
        {
            while (getline(is, fileID))
            {
                boost::trim_right(fileID);

                filePath = Config::HEATMAPS_GREY_DIR;
                filePath += fileID;
                filePath += Config::THERMAL_IMAGE_FILE_EXT;

                files[fileID] = filePath;
            }
        }
    }

    // Load test data and ground truth
    void DatasetLoader::LoadTestData(std::map<std::string, std::string>& testFiles, std::map<std::string, std::vector<Recognition::FumaroleDetectionResult>>& groundTruth)
    {
        // get the test files
        GetTestFiles(testFiles);

        // load the bounding box ground truth data
        boost::property_tree::ptree ptree;
        const std::string parentNode { "annotation" };

        int x = 0, y = 0;
        int width = 0, height = 0;
        std::string filePath;

        for (std::map<std::string, std::string>::const_iterator iter = testFiles.begin(); iter != testFiles.end(); iter++)
        {
            filePath = Config::TEST_GROUND_TRUTH_BOUNDING_BOX_XML_DIR + iter->first + ".xml";

            if (boost::filesystem::exists(filePath))
            {
                boost::property_tree::read_xml(filePath, ptree);

                // get all the fumarole bounding boxes for this file
                for (boost::property_tree::ptree::value_type &child : ptree.get_child(parentNode))
                {
                    if (child.first == "object")
                    {
                        x = child.second.get<int>("bndbox.xmin");
                        y = child.second.get<int>("bndbox.ymin");
                        width = child.second.get<int>("bndbox.xmax") - x;
                        height = child.second.get<int>("bndbox.ymax") - y;

                        Recognition::FumaroleDetectionResult r;
                        r.BoundingBox = std::move(cv::Rect(x, y, width, height));
                        r.ImageID = iter->first;

                        groundTruth[iter->first].emplace_back(r);
                    }
                }
            }
        }
    }
}
