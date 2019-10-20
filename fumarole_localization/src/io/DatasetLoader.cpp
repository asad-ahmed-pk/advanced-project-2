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
    void DatasetLoader::GetTestFileIDs(std::vector<std::string> &fileIDs)
    {
        std::ifstream is;
        is.open(Config::TEST_ID_LIST_PATH, std::ios::in);

        std::string fileID;
        if (is.is_open())
        {
            while (getline(is, fileID))
            {
                boost::trim_right(fileID);
                fileIDs.emplace_back(fileID);
            }
        }
    }

    // Bounding box load (testing data)
    void DatasetLoader::LoadTestData(std::map<std::string, std::vector<cv::Rect>>& detections)
    {
        // get the testing file IDs
        std::vector<std::string> fileIDs;
        GetTestFileIDs(fileIDs);

        // load the bounding box ground truth data
        boost::property_tree::ptree ptree;
        const std::string parentNode { "annotation" };

        int x = 0, y = 0;
        int width = 0, height = 0;
        std::string filePath;

        for (const std::string& fileID : fileIDs)
        {
            filePath = Config::TEST_GROUND_TRUTH_BOUNDING_BOX_XML_DIR + fileID + ".xml";
            std::vector<cv::Rect> boundingBoxes;

            if (boost::filesystem::exists(filePath))
            {
                boost::property_tree::read_xml(filePath, ptree);

                // get all the fumarole bounding boxes for this file
                for (boost::property_tree::ptree::value_type &child : ptree.get_child(parentNode)) {
                    if (child.first == "object") {
                        x = child.second.get<int>("bndbox.xmin");
                        y = child.second.get<int>("bndbox.ymin");
                        width = child.second.get<int>("bndbox.xmax") - x;
                        height = child.second.get<int>("bndbox.ymax") - y;

                        boundingBoxes.push_back(std::move(cv::Rect(x, y, width, height)));
                    }
                }
            }

            detections[fileID] = std::move(boundingBoxes);
        }
    }
}
