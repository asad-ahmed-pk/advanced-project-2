// main.cpp
// Main executable

#include <string>
#include <iostream>
#include <map>

#include "model/FumaroleType.hpp"
#include "pipeline/Pipeline.hpp"
#include "io/fumarole_data_io.hpp"

int main(int argc, char** argv)
{
    // get files
    std::map<std::string, std::string> files;
    IO::GetGreyscaleHeatMaps(files);

    // construct pipelines
    std::cout << "\n\nRunning pipeline for very hot fumaroles" << std::endl;
    Pipeline::Pipeline pVeryHot(files, Model::FumaroleType::FUMAROLE_VERY_HOT);
    pVeryHot.Run();

    std::cout << "\n\nRunning pipeline for hot fumaroles" << std::endl;
    Pipeline::Pipeline pHot(files, Model::FumaroleType::FUMAROLE_HOT);
    pHot.Run();

    std::cout << "\nDone" << std::endl;

    return 0;
}

