//
// DatasetLoader.cpp
// Loads data from the file system
//

#include "io/DatasetLoader.hpp"
#include "config/config.hpp"

#include <iostream>

#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

namespace IO
{
    // Load test file IDs in the given folder
    void DatasetLoader::GetTestFiles(const std::string& folder, std::map<std::string, std::string>& files)
    {
        std::ifstream is;
        is.open(Config::RESOURCES_DIR + folder + Config::TEST_ID_LIST_FILE_NAME, std::ios::in);

        std::string fileID;
        std::string filePath;

        if (is.is_open())
        {
            while (getline(is, fileID))
            {
                boost::trim_right(fileID);

                filePath = Config::THERMAL_IMAGES_DIR;
                filePath += Config::THERMAL_IMAGE_PREFIX;
                filePath += fileID;
                filePath += Config::THERMAL_IMAGE_FILE_EXT;

                files[fileID] = filePath;
            }
        }
    }

    // Load test data and ground truth
    void DatasetLoader::LoadTestData(const std::string& folder, std::map<std::string, std::string>& testFiles, std::map<std::string, std::vector<Detection::FumaroleDetection>>& groundTruth)
    {
        // get the test files
        GetTestFiles(folder, testFiles);

        // load the bounding box ground truth data
        boost::property_tree::ptree ptree;
        const std::string parentNode { "annotation" };

        int x = 0, y = 0;
        int width = 0, height = 0;
        std::string classString;
        std::string filePath;

        for (std::map<std::string, std::string>::const_iterator iter = testFiles.begin(); iter != testFiles.end(); iter++)
        {
            filePath = Config::RESOURCES_DIR + folder + iter->first + ".xml";

            // to init the key (even if no fumaroles exist for this image)
            groundTruth[iter->first];

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

                        Detection::FumaroleDetection r;
                        r.BoundingBox = std::move(cv::Rect(x, y, width, height));

                        classString = child.second.get<std::string>("name");
                        r.Type = GetType(classString);

                        groundTruth[iter->first].emplace_back(r);
                    }
                }
            }
        }
    }

    // Get type enum from the class string id
    Model::FumaroleType DatasetLoader::GetType(const std::string& classString)
    {
        if (classString == "hole") {
            return Model::FumaroleType::FUMAROLE_HOLE;
        }
        else if (classString == "open_vent") {
            return Model::FumaroleType::FUMAROLE_OPEN_VENT;
        }
        else if (classString == "hidden_vent") {
            return Model::FumaroleType::FUMAROLE_HIDDEN_VENT;
        }
        else if (classString == "heated_area") {
            return Model::FumaroleType::FUMAROLE_HEATED_AREA;
        }
        else {
            return Model::FumaroleType::UNKNOWN;
        }
    }
}
