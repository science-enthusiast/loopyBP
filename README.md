# Stereo via sum product BP

This project implements sum product belief propagation (BP) for stereo depth estimation.

## Setup

OpenCV needs to be installed.


Keep track of the folders where the header files and library files of OpenCV can be found. For e.g., on Ubuntu 20.04 the following folders `/usr/include/opencv4` and `/usr/include/opencv4/opencv2` are required for the header files and `/usr/lib/x86_64-linux-gnu/` is required for linking with the library files.


The following libraries need to be linked with: `opencv_core` `opencv_imgcodecs`

## Usage

Compile the project to obtain the executable. For e.g., here is a command that works with Ubuntu 20.04: `g++ -I/usr/include/opencv4/opencv2 -I/usr/include/opencv4 -L/usr/lib/x86_64-linux-gnu/ main.cpp -lopencv_core -lopencv_imgcodecs`.


The executable has to be run with three arguments: the left image, the right image (in that order) and maximum disparity in pixels. It is easy to figure out maximum disparity using a tool like GIMP and viewing the images as layers. Here is an example based on Venus from the Middlebury dataset: `./a.out data/venus/im0.ppm data/venus/im1.ppm 4`

## Discussion

The code is not yet optimized. Expect slow speed.


The memory requirement is image size times maximum disparity. It increases linearly with image size and the maximum disparity, since, each pixel is assigned a marginal probability for each of the possible disparities. Right now, all the data is expected to reside in the RAM. An out-of-core extension will be an interesting future version.
