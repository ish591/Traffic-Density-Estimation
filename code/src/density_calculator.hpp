 #include <iostream>
#include <opencv2/opencv.hpp>
#include <fstream>

using namespace std;
using namespace cv;
float pref1 = 0.0;
float pref2 = 0.0;
float pref3 = 0.0;
float pref4 = 0.0;
//The function below computes dynamic queue density
float compute_dense_flow(Mat frame_cropped_previous, Mat frame_cropped_next, int total_pixels)
{
    Mat flow;
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
    //imshow("dynamicMasked", dynamic_mat_color);
    return dynamic_density;
}

pair<float, vector<Point2f>> compute_sparse_flow(Mat frame_cropped_previous, Mat frame_cropped_next, int total_pixels, vector<Point2f> p0, vector<Point2f> p1, vector<Scalar> colors, Mat colored_next_frame)
{

    float ans = 0;
    Mat mask = Mat::zeros(colored_next_frame.size(), colored_next_frame.type());
    vector<uchar> status;
    vector<float> err;
    TermCriteria criteria = TermCriteria((TermCriteria::COUNT) + (TermCriteria::EPS), 10, 0.03);
    calcOpticalFlowPyrLK(frame_cropped_previous, frame_cropped_next, p0, p1, status, err, Size(15, 15), 2, criteria);
    //vector<Point2f> new_points;
    for (uint i = 0; i < p0.size(); i++)
    {
        // Select good points
        if (status[i] == 1)
        {
            ans += (float)sqrt((p1[i].x - p0[i].x) * (p1[i].x - p0[i].x) + (p1[i].y - p0[i].y) * (p1[i].y - p0[i].y));
            //new_points.push_back(p1[i]);
            // draw the tracks
            line(mask, p1[i], p0[i], colors[i], 2);
            circle(colored_next_frame, p1[i], 5, colors[i], -1);
        }
    }
    Mat img;
    //cout<<colored_next_frame.size();
    add(colored_next_frame, mask, img);
    //imshow("Frame", img);
    waitKey(10);
    ans = (float)ans / 5000.0;
    //if (new_points.size()==0){new_points = p0;}
    vector<Point2f> p_next;
    goodFeaturesToTrack(frame_cropped_next, p_next, 100, 0.3, 7, Mat(), 7, false, 0.04);
    if (p_next.size() == 0)
    {
        return {ans, p0};
    }
    return {ans, p_next};
}
pair<pair<float, vector<Point2f>>, Mat> compute_dynamic(Mat frame, Mat frame_cropped_previous, Mat homography, Rect crop_coordinates, int total_pixels, bool sparse, vector<Point2f> p0, vector<Point2f> p1, vector<Scalar> colors)
{
    Mat colored_next_frame, frame_cropped_next;
    warpPerspective(frame, colored_next_frame, homography, frame.size()); //warping and cropping the next frame
    colored_next_frame = colored_next_frame(crop_coordinates);
    Mat flow(frame_cropped_previous.size(), CV_32FC2);
    cvtColor(colored_next_frame, frame_cropped_next, cv::COLOR_BGR2GRAY);
    //imshow("p",frame_cropped_next);
    //waitKey(0);
    if (sparse == true)
    {
        pair<float, vector<Point2f>> sparse_res = compute_sparse_flow(frame_cropped_previous, frame_cropped_next, total_pixels, p0, p1, colors, colored_next_frame);
        return {{sparse_res.first, sparse_res.second}, frame_cropped_next};
    }
    else
    {
        vector<Point2f> empty;
        return {{compute_dense_flow(frame_cropped_previous, frame_cropped_next, total_pixels), empty}, frame_cropped_next};
    }
}

//The function below computes static queue density
float compute_static(Mat frame, Mat homography, Rect crop_coordinates, Ptr<BackgroundSubtractor> pBackSub, int total_pixels)
{
    cvtColor(frame, frame, cv::COLOR_BGR2GRAY);
    Mat frame_warped, frame_cropped, frame_mask;
    warpPerspective(frame, frame_warped, homography, frame.size()); //warping, cropping and applying background subtraction
    frame_cropped = frame_warped(crop_coordinates);
    pBackSub->apply(frame_cropped, frame_mask, 0.0); //apply background mask with 0 learning rate to keep static empty background
    // Mat frame_bg;
    // pBackSub->getBackgroundImage(frame_bg);
    // imshow("bg", frame_bg);

    // //show the frame in the created window
    //imshow("cropped", frame_cropped);
    //imshow("masked", frame_mask);
    float static_density = ((float)countNonZero(frame_mask)) / total_pixels; //computing static queue density
    return static_density;
}

//function called from main to compute both the densities
void density_calculator(string video_filename, Mat homography, Rect crop_coordinates, Mat frame_empty, int skip, int start_frame, int end_frame, string out_filename, int width, int height, bool sparse)
{
    VideoCapture cap(video_filename);
    if (cap.isOpened() == false)
    {
        cout << "Cannot open the video file!" << endl;
        cin.get(); // wait for any key press
        return;
    }
    //Mat image;
    // cap.read(image);
    //resize(image, image, Size(640, 360), 0, 0, INTER_CUBIC);
    // namedWindow("firstframe", 1);
    // imshow("firstframe", image);
    // waitKey(0);
    //opening the output file
    ofstream fout;
    fout.open(out_filename);
    //resize(frame_empty, frame_empty, Size(width, height), 0, 0, INTER_CUBIC);
    fout << "frame_number static_density dynamic_density" << endl;
    Mat frame_empty_processed;
    Ptr<BackgroundSubtractor> pBackSub;
    pBackSub = createBackgroundSubtractorMOG2(1, 60, false); //creating the background subtractor using frame_empty as the base
    pBackSub->apply(frame_empty, frame_empty_processed, 1.0);
    //namedWindow("cropped", WINDOW_NORMAL); //create a window for displaying cropped image
    //namedWindow("masked", WINDOW_NORMAL);  //create a window for displaying the mask
    //namedWindow("bg", WINDOW_NORMAL);      //create a window for displaying the background mask

    int total_pixels = frame_empty.rows * frame_empty.cols;
    int framecounter = start_frame;
    Mat frame, frame_previous;

    frame_previous = frame_empty_processed; //previous frame initialization required for dynamic density
    cap.set(CAP_PROP_POS_FRAMES, start_frame);

    vector<Scalar> colors; //for sparse optical flow, random colors used for plotting
    RNG rng;
    for (int i = 0; i < 100; i++)
    {
        int r = rng.uniform(0, 256);
        int g = rng.uniform(0, 256);
        int b = rng.uniform(0, 256);
        colors.push_back(Scalar(r, g, b));
    }

    vector<Point2f> p0, p1; //stores the corner points for sparse optical flow
    goodFeaturesToTrack(frame_empty, p0, 100, 0.3, 7, Mat(), 7, false, 0.04);
    //cout<<p0.size()<<endl;
    //initialising corner points for the first frame
    // Create a mask image for drawing purposes
    while (framecounter < end_frame)
    {
        //this loop iterates over the frames of the video
        bool bSuccess = cap.read(frame); // read a new frame from video
        //cap.set(CAP_PROP_FRAME_WIDTH, 640);
        //cap.set(CAP_PROP_FRAME_HEIGHT, 360);
        if (bSuccess == false)
        {
            cout << "Found the end of the video" << endl;
            break;
        }
        if (!(width == 1920 && height == 1088))
        {
            resize(frame, frame, Size(width, height), 0, 0, INTER_CUBIC);
        }
        //namedWindow("firstframe", 1);
        //imshow("firstframe", frame);
        //framecounter++;
        //imshow("ok",frame);
        //waitKey(1);
        if (framecounter++ % skip != 0) // skipping some frames
            continue;
        //cvtColor(frame, frame, cv::COLOR_BGR2GRAY);
        //computing static and dynamic densities by appropriate function calls
        //creating colors for plotting sparse optical flow
        float static_density = compute_static(frame, homography, crop_coordinates, pBackSub, total_pixels);
        pair<pair<float, vector<Point2f>>, Mat> dynamic_return = compute_dynamic(frame, frame_previous, homography, crop_coordinates, total_pixels, sparse, p0, p1, colors);
        float dynamic_density = dynamic_return.first.first;

        if (waitKey(1) == 27)
        {
            cout << "Esc key is pressed by user. Stopping the video" << endl;
            break;
        }
        //outputting the densities on console as well as an output file
        cout << framecounter << ": " << static_density << " " << dynamic_density << endl;
        fout << framecounter << " " << static_density << " " << dynamic_density << endl;
        frame_previous = dynamic_return.second;
        p0 = dynamic_return.first.second;
    }
    fout.close();
}
