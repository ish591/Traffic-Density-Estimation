#include <iostream>
#include <opencv2/opencv.hpp>
#include <fstream>

using namespace std;
using namespace cv;

//The function below computes dynamic queue density
pair<float, Mat> compute_dynamic(Mat frame, Mat frame_cropped_previous, Mat homography, Rect crop_coordinates, int total_pixels)
{
    Mat frame_cropped_next;
    warpPerspective(frame, frame_cropped_next, homography, frame.size()); //warping and cropping the next frame
    frame_cropped_next = frame_cropped_next(crop_coordinates);
    Mat flow(frame_cropped_previous.size(), CV_32FC2);                                                    //initializes a flow matrix of two channels (for flows in x and y directions)
    calcOpticalFlowFarneback(frame_cropped_previous, frame_cropped_next, flow, 0.5, 3, 15, 3, 7, 1.5, 0); //calculates the optical flow

    // visualization
    Mat flow_parts[2];
    split(flow, flow_parts);                                           //split the flow matrix into 2 channels
    Mat magnitude, angle, magn_norm;                                   //magnitude of flow, angle, and normalized magnitude matrices
    cartToPolar(flow_parts[0], flow_parts[1], magnitude, angle, true); //converts cartesian flows in x and y to polar forms in r and theta
    //true is for angle in degrees
    normalize(magnitude, magn_norm, 0.0f, 1.0f, NORM_MINMAX); //normalise magnitudes in the range 0-1 linearly
    angle *= ((1.f / 360.f) * (180.f / 255.f));               //angle computation

    //build hsv image
    Mat _hsv[3], hsv, hsv8, dynamic_mat_color, dynamic_mat_bw;
    _hsv[0] = angle;
    _hsv[1] = Mat::ones(angle.size(), CV_32F);
    _hsv[2] = magn_norm;
    merge(_hsv, 3, hsv);               //merge all into a single 3 channel matrix
    hsv.convertTo(hsv8, CV_8U, 255.0); //convert hsv matrix to a matrix in pixel range of 0-255
    cvtColor(hsv8, dynamic_mat_color, COLOR_HSV2BGR);
    cvtColor(dynamic_mat_color, dynamic_mat_bw, COLOR_BGR2GRAY);                  //converting into a grayscale matrix
    dynamic_mat_bw = dynamic_mat_bw > 12;                                         //threshold to convert grayscal to pure black-white image
    float dynamic_density = ((float)countNonZero(dynamic_mat_bw)) / total_pixels; //computation of dynamic density
    imshow("dynamicMasked", dynamic_mat_color);
    return {dynamic_density, frame_cropped_next};
}

//The function below computes static queue density
float compute_static(Mat frame, Mat homography, Rect crop_coordinates, Ptr<BackgroundSubtractor> pBackSub, int total_pixels)
{
    Mat frame_warped, frame_cropped, frame_mask;
    warpPerspective(frame, frame_warped, homography, frame.size()); //warping, cropping and applying background subtraction
    frame_cropped = frame_warped(crop_coordinates);
    pBackSub->apply(frame_cropped, frame_mask, 0.0); //apply background mask with 0 learning rate to keep static empty background

    //show the frame in the created window
    imshow("cropped", frame_cropped);
    imshow("masked", frame_mask);
    float static_density = ((float)countNonZero(frame_mask)) / total_pixels; //computing static queue density
    return static_density;
}

void density_calculator(VideoCapture cap, Mat homography, Rect crop_coordinates)
//function called from main to compute both the densities
{
    //opening the output file
    ofstream fout;
    fout.open("./results/out.txt", std::ofstream::out | std::ofstream::trunc);

    Mat frame_empty;
    cap.set(CAP_PROP_POS_FRAMES, 347 * 15); //capturing a frame with no vehicles
    cap.read(frame_empty);

    cvtColor(frame_empty, frame_empty, cv::COLOR_BGR2GRAY);
    warpPerspective(frame_empty, frame_empty, homography, frame_empty.size()); //warping,cropping the background frame
    frame_empty = frame_empty(crop_coordinates);
    double fps = cap.get(CAP_PROP_FPS);

    cout << "Frames per seconds : " << fps << endl;

    Ptr<BackgroundSubtractor> pBackSub;
    pBackSub = createBackgroundSubtractorMOG2(1, 60, false); //creating the background subtractor using frame_empty as the base
    pBackSub->apply(frame_empty, frame_empty, 1.0);
    cap.set(CAP_PROP_POS_FRAMES, 0);
    namedWindow("cropped", WINDOW_NORMAL); //create a window for displaying cropped image
    namedWindow("masked", WINDOW_NORMAL);  //create a window for displaying the mask
    cap.set(CAP_PROP_POS_FRAMES, 0);

    int total_pixels = frame_empty.rows * frame_empty.cols;
    int framecounter = 0;
    Mat frame, frame_previous;
    frame_previous = frame_empty; //previous frame initialization required for dynamic density
    while (true)
    {
        //this loop iterates over the frames of the video

        bool bSuccess = cap.read(frame); // read a new frame from video

        if (bSuccess == false)
        {
            cout << "Found the end of the video" << endl;
            break;
        }
        if (framecounter++ % 5 != 0) //for 3 fps
            continue;
        cvtColor(frame, frame, cv::COLOR_BGR2GRAY);
        //computing static and dynamic densities by appropriate function calls
        float static_density = compute_static(frame, homography, crop_coordinates, pBackSub, total_pixels);
        pair<float, Mat> dynamic_return = compute_dynamic(frame, frame_previous, homography, crop_coordinates, total_pixels);
        float dynamic_density = dynamic_return.first;

        if (waitKey(1) == 27)
        {
            cout << "Esc key is pressed by user. Stopping the video" << endl;
            break;
        }
        //outputting the densities on console as well as an output file
        cout << framecounter << ": " << static_density << " " << dynamic_density << endl;
        fout << framecounter << " " << static_density << " " << dynamic_density << endl;
        frame_previous = dynamic_return.second;
    }
    fout.close();
}
