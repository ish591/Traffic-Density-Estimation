#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

void transform_video(VideoCapture cap, Mat homography, Rect crop_coordinates, Mat frame_empty)
{
    VideoWriter video("trafficvideo_processed.avi", VideoWriter::fourcc('M', 'J', 'P', 'G'), 10, frame_empty.size());

    while (true)
    {
        Mat frame;
        // Capture frame-by-frame
        cap >> frame;
        // If the frame is empty, break immediately
        if (frame.empty())
            break;

        // Write the frame into the file 'outcpp.avi'
        cvtColor(frame, frame, COLOR_BGR2GRAY);
        warpPerspective(frame, frame, homography, frame.size());
        frame = frame(crop_coordinates);
        video.write(frame);

        // Press  ESC on keyboard to  exit
        char c = (char)waitKey(1);
        if (c == 27)
            break;
    }
    video.release();
}

Mat get_empty_frame(VideoCapture cap, int frame_num)
{
    Mat frame_empty;
    cap.set(CAP_PROP_POS_FRAMES, frame_num); //capturing a frame with no vehicles
    cap.read(frame_empty);

    cvtColor(frame_empty, frame_empty, cv::COLOR_BGR2GRAY);

    cap.set(CAP_PROP_POS_FRAMES, 0);
    return frame_empty;
}