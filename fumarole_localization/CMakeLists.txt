# Fumarole Localization Pipeline

# Project
cmake_minimum_required(VERSION 3.10)
project(fumarole_localization)
set(CMAKE_CXX_STANDARD 17)

# OpenCV
find_package(OpenCV 4.1 REQUIRED)

# Boost
find_package(Boost 1.60 COMPONENTS filesystem REQUIRED)
include_directories(${Boost_INCLUDE_DIR})

# Includes
include_directories(include)

# Sources
list(APPEND PIPELINE_SOURCES
        src/pipeline/PipelineElement.cpp
        src/pipeline/HeatThreshold.cpp
        src/pipeline/FumaroleContour.cpp
        src/pipeline/FumaroleLocalizer.cpp
        src/pipeline/Pipeline.cpp
)

list(APPEND IO_SOURCES
        src/io/fumarole_data_io.cpp
        src/io/DatasetLoader.cpp
)

list(APPEND OTHER_SOURCES
        src/config/ConfigParser.cpp
        src/recognition/FumaroleRecognizer.cpp
)

# Executables

# Main program
add_executable(${PROJECT_NAME} src/main.cpp ${PIPELINE_SOURCES} ${IO_SOURCES} ${OTHER_SOURCES})
target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS} ${Boost_LIBRARIES})

# Testing program
add_executable(detector_test src/test.cpp ${PIPELINE_SOURCES} ${IO_SOURCES} ${OTHER_SOURCES})
target_link_libraries(detector_test ${OpenCV_LIBS} ${Boost_LIBRARIES})
