#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

void transform_video(VideoCapture cap, Mat homography, Rect crop_coordinates)
{
    // VideoWriter video("trafficvideo_processed.avi", cv::VideoWriter::fourcc('M', 'J', 'P', 'G'), 10, Size(crop_coordinates.width, crop_coordinates.height));
    // Mat frame;
    // while (true)
    // {
    //     //this loop iterates over the frames of the video

    //     bool bSuccess = cap.read(frame); // read a new frame from video
    //     if (bSuccess == false)
    //     {
    //         cout << "Found the end of the video" << endl;
    //         break;
    //     }
    //     cvtColor(frame, frame, cv::COLOR_BGR2GRAY);
    //     //warping ,cropping the background frame
    //     warpPerspective(frame, frame, homography, frame.size());
    //     frame = frame(crop_coordinates);
    //     video.write(frame);
    // }

    // maybe return a VideoCapture object here.
}

Mat get_empty_frame(VideoCapture cap, int frame_num, Mat homography, Rect crop_coordinates)
{
    Mat frame_empty;
    cap.set(CAP_PROP_POS_FRAMES, frame_num); //capturing a frame with no vehicles
    cap.read(frame_empty);

    cvtColor(frame_empty, frame_empty, cv::COLOR_BGR2GRAY);
    warpPerspective(frame_empty, frame_empty, homography, frame_empty.size()); //warping,cropping the background frame
    frame_empty = frame_empty(crop_coordinates);
    cap.set(CAP_PROP_POS_FRAMES, 0);
    return frame_empty;
}