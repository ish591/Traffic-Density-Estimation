/*
Made by:
Aman Verma   : 2019CS50419
Ishaan Singh : 2019CS10359
*/

#include <iostream>
#include <opencv2/opencv.hpp>
#include <fstream>
#include <utility>
#include <regex>
// #include "density_calculator.hpp"
#include "process_video.hpp"
#include "analysis_methods.hpp"

using namespace std;
using namespace cv;

// stores the selected points as pairs of integers
vector<pair<int, int>> selected_pts;
// mouse callback function, is executed when the user clicks on the initial image to select points
void clickEvent(int event, int x, int y, int flags, void *params)
{

    Mat *im_empty = (Mat *)params;
    if (event == EVENT_LBUTTONDOWN)
    {
        cout << x << " " << y << endl;
        circle(*im_empty, Point2f(x, y), 9, Scalar(20, 20, 20), -1); // to display a circular dot at points of selection
        selected_pts.push_back({x, y});
        imshow("Empty", *im_empty);
    }
}

// returns the selected points of the source image in Point2f form, after sorting them in anti-clockwise fashion
vector<Point2f> get_src_points()
{
    vector<Point2f> pts_src;
    sort(selected_pts.begin(), selected_pts.end());
    if (selected_pts[0].second > selected_pts[1].second)
    {
        pair<int, int> u = selected_pts[0];
        selected_pts[0] = selected_pts[1];
        selected_pts[1] = u;
    }
    if (selected_pts[3].second > selected_pts[2].second)
    {
        pair<int, int> u = selected_pts[2];
        selected_pts[2] = selected_pts[3];
        selected_pts[3] = u;
    }
    for (int i = 0; i < 4; i++)
    {
        pts_src.push_back(Point2f(selected_pts[i].first, selected_pts[i].second));
    }
    return pts_src;
}

// The function below returns the coordinates of the points to which the selected points are to be mapped for warping
vector<Point2f> get_dst_points(vector<Point2f> pts_src)
{
    vector<Point2f> pts_dst;
    // The below code dynamically maps the points in an attempt to maintain the aspect ratio
    int left_x = pts_src[0].x;
    int top_y = (pts_src[0].y + pts_src[3].y) / 2;
    int right_x = pts_src[3].x;
    int bottom_y = (pts_src[1].y + pts_src[2].y) / 2;

    pts_dst.push_back(Point2f(left_x, top_y));
    pts_dst.push_back(Point2f(left_x, bottom_y));
    pts_dst.push_back(Point2f(right_x, bottom_y));
    pts_dst.push_back(Point2f(right_x, top_y));
    return pts_dst;
}
pair<int,pair<int,int>> get_args(vector<int>params){
    switch (params[0]){
        case 0:{
            if (params.size()==1){return {0,{0,0}};}
            break;
        }
        case 1:{
            if (params.size()==2){
                if (params[1]>=5 && params[1]<=5736){return {1,{params[1],0}};}
                break;
            }
            break;
        }
        case 2:{
            if (params.size()==3){
                if(params[1]>=1 && params[1]<=1920 && params[2]>=1 && params[2]<=1088){
                    return {2,{params[1],params[2]}};
                }
                break;
            }
            break;
        }
        case 3:{
            if (params.size()==2){
                if (params[1]>=1){
                    return {3,{params[1],0}};
                }
                break;
            }
            break;
        }
       case 4:{
           if (params.size()==2){
               if (params[1]>=1){
                   return {4,{params[1],0}};
               }
               break;
           }
           break;
       }
      case 5:{
          if (params.size()==2){
              if (params[1]==1 || params[1]==0){
                  return {5,{params[1],0}};
              }
              break;
          }
          break;
      }
     default : return {-1,{-1,-1}};
    }
    return {-1,{-1,-1}};
}
int main(int argc, char *argv[])
{
    setNumThreads(1);
    string video_filename = "./assets/trafficvideo.mp4";
    if (argc > 1)
        video_filename = argv[1];

    // Open video file
    VideoCapture cap(video_filename);
    if (cap.isOpened() == false)
    {
        cout << "Cannot open the video file!" << endl;
        cin.get(); // wait for any key press
        return 0;
    }
    // // reading the two images in grayscale
    Mat im_empty = imread("./assets/empty.jpg", IMREAD_GRAYSCALE);
    Mat im_traffic = imread("./assets/traffic.jpg", IMREAD_GRAYSCALE);

    // // creating a copy of the first image and displaying it
    // Mat im_empty_copy = im_empty.clone();
    // imshow("Empty", im_empty_copy);

    // // callback function is executed while the user is selecting the four points
    //while (selected_pts.size() < 4)
    //{
    // setMouseCallback("Empty", clickEvent, (void *)&im_empty_copy);
    // waitKey(200);
    //}
    // // destroys the window of the initial image
    //destroyWindow("Empty");

    // getting source and destination points by calling appropriate functions

    vector<Point2f> pts_src, pts_dst;
    //pts_src = get_src_points();

    //pts_src.push_back(Point2f(961, 230));
    //pts_src.push_back(Point2f(324, 1063));
    //pts_src.push_back(Point2f(1571, 1065));
    //pts_src.push_back(Point2f(1288, 224));
    pts_src.push_back(Point2f(967, 229));
    pts_src.push_back(Point2f(280, 1045));
    pts_src.push_back(Point2f(1517, 1043));
    pts_src.push_back(Point2f(1268, 220));

    pts_dst = get_dst_points(pts_src);
    waitKey(100);

    // finding the homography of the image using openCv functions
    // Mat homography = findHomography(pts_src, pts_dst);

    // Mat im_empty_warped, im_empty_cropped, im_traffic_warped, im_traffic_cropped;

    // warpPerspective(im_empty, im_empty_warped, homography, im_empty.size());
    // warpPerspective(im_traffic, im_traffic_warped, homography, im_traffic.size());

    // // cropping the warped image
    // Rect crop_coordinates = Rect(pts_dst[0].x, pts_dst[0].y, pts_dst[2].x - pts_dst[1].x, pts_dst[1].y - pts_dst[0].y);
    // im_empty_cropped = im_empty_warped(crop_coordinates);
    // im_traffic_cropped = im_traffic_warped(crop_coordinates);

    // // writing the warped and cropped images
    // imwrite("./results/empty_warped.jpg", im_empty_warped);
    // imwrite("./results/empty_cropped.jpg", im_empty_cropped);
    // imwrite("./results/traffic_warped.jpg", im_traffic_warped);
    // imwrite("./results/traffic_cropped.jpg", im_traffic_cropped);

    Mat frame_empty = get_empty_frame(cap, 347 * 15);
    
    //regex expressions used to parse the input file
    // transform_video(cap, homography, crop_coordinates, frame_empty);

    int total_frames = cap.get(CAP_PROP_FRAME_COUNT);
    double fps = cap.get(CAP_PROP_FPS);
    cout << "Number of frames: " << total_frames << endl;
    cout << "Frames per seconds : " << fps << endl;
    
    ifstream fin("./src/input.txt");
    string line;
    int count =0;
    ofstream fout;
    ofstream fout1;
    fout.open("./results/plotted_files.txt");
    fout1.open("./results/utility_runtime.txt");
    while(getline(fin, line)){
        istringstream ss(line);
        vector<int>params;
        int n;
        while(ss>>n){
            params.push_back(n);
        }
        pair<int,pair<int,int>> ag = get_args(params);
        count++;
        if (ag.first == -1){
            cout<<"Invalid input file\n";
            return 0;
        }
        else{
            cout<<"Output for function number: "<<count<<"\n\n";
            string out_file_name = "./results/method_"+to_string(ag.first)+"_function_"+to_string(count)+".txt";
            fout << out_file_name <<endl;
            cout<<out_file_name<<endl;
            call_method(ag.first, video_filename, out_file_name, pts_src, pts_dst, frame_empty, total_frames, fout1, ag.second.first, ag.second.second);
            cout<<endl;
        }
    }
    fout.close();
    fout1.close();
    return 0;
}
