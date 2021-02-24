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

### Additional Commands

A few additional make rules:

```bash
make clean # To clean the build and runtime files run.
make help  # To see all make options.
```

An incorrect make rule helps the user by directing them to the 'make help' command.

## Functionality

### Current

- Select points on an empty road for configuration.
- Returns a top view of the road by applying a homography computed using OpenCV.
- Crops the appropriate region and outputs various views with the applied homography.

### Salient Features

- Shows the marked points on the image as the user selects them for convenience.
- Points can be selected in any order and are mapped appropriately.
- Calculates the warp destination coordinates dynamically to incorporate different road sizes in an attempt to maintain the aspect ratio.

## Developers

- Aman Verma : 2019CS50419 <br>
- Ishaan Singh : 2019CS10359
