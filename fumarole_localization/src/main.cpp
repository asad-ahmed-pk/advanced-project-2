//
// main.cpp
// Main program executable
//

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>

#include <boost/filesystem.hpp>

#include "model/FumaroleType.hpp"
#include "detection/FumaroleDetector.hpp"

const int REQ_PARAMS_COUNT = 2;

const std::vector<std::string> SUPPORTED_FILE_TYPES = {
        ".jpeg", ".jpg", ".JPG", ".JPEG",
        ".png", ".PNG",
        ".bmp", ".BMP",
        ".exr"
};

void WriteDetectionsToCSVFile(const std::map<std::string, std::vector<Detection::FumaroleDetection>>& detections, const std::string& outputDir);

int main(int argc, char** argv)
{
    // required params check
    if (argc < REQ_PARAMS_COUNT) {
        std::cout << "\nUsage: fumarole_localization [file path for directory of thermal images] [optional: output folder path]\n" << std::endl;
        return 1;
    }

    // get params and optional params
    std::string thermalImagesDir { argv[1] };

    // param 3 is optional output dir
    std::string csvOutputDir { "detector_csv_output" };
    if (argc == 3) {
        csvOutputDir = argv[2];
    }

    // load images from directory
    std::map<std::string, std::string> files;

    std::string ext;

    boost::filesystem::directory_iterator iterEnd;
    for (boost::filesystem::directory_iterator iter(thermalImagesDir); iter != iterEnd; iter++)
    {
        if (boost::filesystem::is_regular_file(iter->path()))
        {
            // skip dot files
            ext = iter->path().extension().string();
            if (std::find(SUPPORTED_FILE_TYPES.begin(), SUPPORTED_FILE_TYPES.end(), ext) != SUPPORTED_FILE_TYPES.end()) {
                files[iter->path().stem().string()] = iter->path().string();
            }
        }
    }

    // create detector with no intermediate output and run detector
    std::map<std::string, std::vector<Detection::FumaroleDetection>> detections;

    Detection::FumaroleDetector detector(false);
    detector.DetectFumaroles(files, detections);

    // write to csv files
    std::cout << "\n\nImages processed. Writing detections to CSV files to " << csvOutputDir << std::endl;
    WriteDetectionsToCSVFile(detections, csvOutputDir);

    return 0;
}

// write the detections to the csv file
void WriteDetectionsToCSVFile(const std::map<std::string, std::vector<Detection::FumaroleDetection>>& detections, const std::string& outputDir)
{
    // create output dir if needed
    if (!boost::filesystem::exists(outputDir)) {
        boost::filesystem::create_directories(outputDir);
    }

    std::string path;

    for (const auto& pair : detections)
    {
        // create full path for the csv file for this image
        path += outputDir;
        path += "/";
        path += pair.first;
        path += ".csv";

        // write detections for this csv file
        std::ofstream fs(path, std::ios::out);

        // write header
        fs << "x_min,x_max,y_min,y_max,width,height,class_label";

        // write each detection as a record
        for (const auto& d : pair.second)
        {
            fs << "\n";
            fs << d.BoundingBox.x << ",";
            fs << d.BoundingBox.x + d.BoundingBox.width << ",";
            fs << d.BoundingBox.y << ",";
            fs << d.BoundingBox.y + d.BoundingBox.height << ",";
            fs << d.BoundingBox.width << ",";
            fs << d.BoundingBox.height << ",";
            fs << Model::TypeNameString(d.Type);
        }

        // clear path
        path.clear();
    }
}

