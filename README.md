# Advanced Project II
## A Computer Vision Approach for Fumarole Detection And Classification

The developed fumarole detection and classification pipeline for Jacobs University Advanced Project II module in MSc Data Engineering.

### Requirements

This is a CMake, C++17 based program. The following libraries are required:

1. [OpenCV 4.1+](https://opencv.org/)
2. [Boost 1.60+](https://www.boost.org/)
3. [Eigen3](http://eigen.tuxfamily.org/index.php?title=Main_Page)

### Installation

Using CMake as the build system:

```bash
git clone https://github.com/asadahmedde/advanced-project-2.git
cd advanced-project-2
cd fumarole_localization
mkdir build && cd build
cmake ..
make fumarole_localization
```

### Usage

```bash
./fumarole_localization thermal_images_dir (optional_output_dir)
```

**Note**: The images must be in greyscale.

CSV files will be saved in the provided output directory or in *detector_csv_output* in the executable directory by default. Each CSV file will be named with the corresponding name of the image file in the input directory and will contain a list of bounding boxes for the detections + the class label. 

### License
[MIT](https://choosealicense.com/licenses/mit/)