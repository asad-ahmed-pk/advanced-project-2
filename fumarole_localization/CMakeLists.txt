# Fumarole Localization Pipeline

# Project
cmake_minimum_required(VERSION 3.10)
project(fumarole_localization)
set(CMAKE_CXX_STANDARD 14)

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
        src/pipeline/Pipeline.cpp
)

list(APPEND IO_SOURCES
        src/io/fumarole_data_io.cpp
)

# Executables
add_executable(${PROJECT_NAME} src/main.cpp ${PIPELINE_SOURCES} ${IO_SOURCES} src/config/ConfigParser.cpp)
target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS} ${Boost_LIBRARIES})
