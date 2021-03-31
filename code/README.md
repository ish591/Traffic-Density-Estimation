## Testing

### Build

Make sure OpenCV has been installed and is configured with pkg-config. Clone or extract the repository and run

```bash
cd Traffic-Density-Estimation/
cd Code/
make # To compile src/main.cpp
```

### Downloading assets

```bash
make download # To download the video and place it in the appropriate directory appropriately.
# In case you already have the video, copy it into the assets directory.
cp path/to/mp4/video/file ./Assets/trafficvideo.mp4
```

### Run

To execute the compiled file run (Recommended):

```bash
make run # To execute ./build/main.out
```

### Additional Commands

A few additional make rules:

```bash
make clean      # To clean the build and runtime files run.
make help       # To see all make options.
```

An incorrect make rule helps the user by directing them to the 'make help' command.
