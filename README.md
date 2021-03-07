# Traffic Density Estimation

A tool for estimating traffic density using traffic-cams.

## Testing

### Build

Make sure OpenCV has been installed and is configured with pkg-config. Clone or extract the repository and run

```bash
cd Traffic-Density-Estimation
make # To compile src/main.cpp
```

### Downloading assets

```bash
make download # To download the video and place it in the appropriate directory appropriately.
# In case you already have the video, copy it into the assets directory.
cp path/to/mp4/video/file assets/trafficvideo.mp4
```

### Run

To execute the compiled file run (Recommended):

```bash
make run # To execute ./build/main.out
```

Alternatively, you can run

```bash
# Do not cd into the build folder to run the executable
./build/main.out # runs ./assets/trafficvideo.mp4 by default
# you can also provide the relative or absolute path to a video file to run it
./build/main.out assets/trafficvideo.mp4
```

### Additional Commands

A few additional make rules:

```bash
make clean      # To clean the build and runtime files run.
make help       # To see all make options.
make graph      # Plot graph with results in out.txt
make download   # To download the video and store is in the assets directory
```

An incorrect make rule helps the user by directing them to the 'make help' command.

## Functionality

### Current

- Select points on an empty road for configuration.
- Returns a top view of the road by applying a homography computed using OpenCV.
- Crops the appropriate region and outputs various views with the applied homography.
- Determines static density by using background subtraction with an empty frame.
- Determines dynamic density by using optical flow across frames.

### Salient Features

- Shows the marked points on the image as the user selects them for convenience.
- Points can be selected in any order and are mapped appropriately.
- Calculates the warp destination coordinates dynamically to incorporate different road sizes in an attempt to maintain the aspect ratio.
- Displays cropped frame as well as the computedstatic and dynamic masks.
- Automated commands for easier evaluation.

## Developers

- Aman Verma : 2019CS50419 <br>
- Ishaan Singh : 2019CS10359
