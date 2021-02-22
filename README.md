# Traffic Density Estimation

A tool for estimating traffic density using traffic-cams.

## Testing
### Build
Make sure OpenCV has been installed and is configured with pkg-config. Clone or extract the repository and run
```bash
cd Traffic-Density-Estimation
make # To compile src/main.cpp
```
### Run
To execute the compiled file run (Recommended): 
```bash 
make run # To execute ./build/main.out
```
Alternatively, you can run
```bash
# Do not cd into the build folder to run the executable
./build/main.out 
```
### Clean
To clean the build and runtime files run: 
```bash
make clean
```

## Functionality
### Current
- Select points on an empty road for configuration.
- Returns a top view of the road by applying a homography computed using OpenCV.
- Crops the appropriate region and outputs various views with the applied homography.
### Salient Features
- Shows the marked points on the image as the user selects them for convenience.
- Points can be selected in any order and are mapped appropriately. 
- Calculates the warp destination coordinates dynamically to incorporate different road sizes in an attempt to maintain the aspect ratio.
