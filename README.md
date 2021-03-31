# Traffic Density Estimation

A tool for estimating traffic density using traffic-cams.

## Functionality

### Current

- Select points on an empty road for configuration.
- Returns a top view of the road by applying a homography computed using OpenCV.
- Crops the appropriate region and outputs various views with the applied homography.
- Determines static density by using background subtraction with an empty frame.
- Determines dynamic density by using optical flow across frames.
- Implemented using various methods to perform trade-off analyses.
  - Method 0: Running standard, unoptimised code for baseline calculation.
  - Method 1: Optimised by skipping frames.
  - Method 2: Optimised by reducing resolution.
  - Method 3: Optimised by multithreading - splitting frames spatially.
  - Method 4: Optimised by multithreading - temporal splitting of video.
  - Method 5: Sparse vs Dense optical flow.

### Salient Features

- Shows the marked points on the image as the user selects them for convenience.
- Allows selection of points in arbitrary order.
- Calculates the warp destination coordinates dynamically to incorporate different road sizes in an attempt to maintain the aspect ratio.
- Implements optical flow and compares dense vs sparse implementations.
- Automated commands for easier evaluation.

## Test

To run the code cd into the Code directory and check out [code/README.md](code/README.md)

## Analysis

To see the results and analysis check out [analysis/README.md](analysis/README.md)

## Developers

- Aman Verma : 2019CS50419 <br>
- Ishaan Singh : 2019CS10359
