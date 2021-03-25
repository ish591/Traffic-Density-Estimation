#include <iostream>
#include <opencv2/opencv.hpp>
#include <fstream>
#include <utility>
#include "density_calculator.hpp"
#include <thread>
#define BASE_SKIP 5
using namespace std;
using namespace cv;

void method0(string video_filename, Mat homography, Rect crop_coordinates, Mat frame_empty, int total_frames)
{
    density_calculator(video_filename, homography, crop_coordinates, frame_empty, BASE_SKIP, 0, total_frames, "out_0", 1920, 1088, 0);
}
void method1(string video_filename, Mat homography, Rect crop_coordinates, Mat frame_empty, int total_frames, int skip_factor)
{
    //process frame N, then N+skip_frames ..
    density_calculator(video_filename, homography, crop_coordinates, frame_empty, skip_factor * BASE_SKIP ,0, total_frames,"out_1_skipped", 1920, 1088, 0);
    //for intermediate frames use value of N
     ofstream fout("./results/out_1.txt");
    ifstream fin("./results/out_1_skipped.txt");
    int a;
    float b, c;
    while (fin >> a)
    {
        fin >> b >> c;
        for(int i=a;i<=min(a+skip_factor * BASE_SKIP - 1,total_frames);i++){
        fout << i << " " << b << " " << c << endl;
        }
    }
    fin.close();
}
void method2(string video_filename, vector<Point2f> pts_src, vector<Point2f> pts_dst, int total_frames, int width, int height, VideoCapture cap){
    for(int i = 0;i < 4; i++){
        pts_src[i].x = ((pts_src[i].x)*width)/1920;
        pts_src[i].y = ((pts_src[i].y)*height)/1088;
        pts_dst[i].x = ((pts_dst[i].x)*width)/1920;
        pts_dst[i].y = ((pts_dst[i].y)*height)/1088;
    }
    Mat homography = findHomography (pts_src, pts_dst);
    Rect crop_coordinates = Rect(pts_dst[0].x, pts_dst[0].y, pts_dst[2].x - pts_dst[1].x, pts_dst[1].y - pts_dst[0].y);
    Mat frame_empty = get_empty_frame(cap, 347 * 15, homography, crop_coordinates);
    density_calculator(video_filename, homography, crop_coordinates, frame_empty, BASE_SKIP, 0, total_frames, "out_2", width, height, 0);
}

void method3(string video_filename, Mat homography, Rect crop_coordinates, Mat frame_empty, int total_frames, int num_threads)
{
    if (num_threads > 5)
        num_threads = 5;
    vector<Rect> crop_coord;
    vector<int> pixels;
    int height = crop_coordinates.height;
    int width = crop_coordinates.width;
    int curr_top_left_x = crop_coordinates.x;
    int curr_top_left_y = crop_coordinates.y;
    for (int i = 0; i < num_threads; i++)
    {
        int curr_height = ((height * (i + 1)) / num_threads) - ((height * i) / num_threads);
        crop_coord.push_back(Rect(curr_top_left_x, curr_top_left_y, width, curr_height));
        curr_top_left_y = curr_top_left_y + curr_height;
        pixels.push_back(curr_height * width);
    }
    vector<thread> threads;
    int y_coord;
    for (int i = 0; i < num_threads; i++)
    {
        if (i == 0)
            y_coord = 0;
        else
            y_coord += crop_coord[i - 1].height;
        threads.push_back(thread(density_calculator, video_filename, homography, crop_coord[i], frame_empty(Rect(0, y_coord, crop_coord[i].width, crop_coord[i].height)), BASE_SKIP, 0, total_frames, "out_3_" + to_string(i), 1920, 1088, 0));
    }
    for (int i = 0; i < num_threads; i++)
        threads[i].join();

    ofstream fout("./results/out_3.txt");
    vector<ifstream> fin;
    int total_pixels = 0;
    for (int i = 0; i < num_threads; i++)
    {
        fin.push_back(ifstream("./results/out_3_" + to_string(i) + ".txt"));
        total_pixels += pixels[i];
    }
    int a;
    float b, c;
    while (fin[0] >> a)
    {
        fin[0] >> b >> c;

        float b_acc = b * pixels[0], c_acc = c * pixels[0];

        for (int i = 1; i < num_threads; i++)
        {
            fin[i] >> a >> b >> c;
            b_acc = b_acc + b * pixels[i];
            c_acc = c_acc + c * pixels[i];
        }
        fout << a << " " << (b_acc / total_pixels) << " " << (c_acc / total_pixels) << endl;
    }
}

void method4(string video_filename, Mat homography, Rect crop_coordinates, Mat frame_empty, int total_frames, int num_threads)
{
    if (num_threads > 5)
        num_threads = 5;
    int frame_limits[num_threads + 1];
    for (int i = 0; i <= num_threads; i++)
    {
        frame_limits[i] = (total_frames * i) / num_threads;
    }
    vector<thread> threads;
    for (int i = 0; i < num_threads; i++)
    {
        threads.push_back(thread(density_calculator, video_filename, homography, crop_coordinates, frame_empty, BASE_SKIP, frame_limits[i], frame_limits[i + 1], "out_4_" + to_string(i), 1920, 1088,0));
    }
    for (int i = 0; i < num_threads; i++)
        threads[i].join();

    ofstream fout("./results/out_4.txt");
    for (int i = 0; i < num_threads; i++)
    {
        ifstream fin("./results/out_4_" + to_string(i) + ".txt");
        string line;
        while (getline(fin, line))
        {
            fout << line << endl;
        }
        fin.close();
    }
}

void bonus_method(string video_filename, Mat homography, Rect crop_coordinates, Mat frame_empty, int total_frames){
    //time_t start,end;
    //time(&start);
    density_calculator(video_filename, homography, crop_coordinates, frame_empty, BASE_SKIP, 0, total_frames, "out_bonus", 1920, 1088, 1);
    // last parameter is a boolean, 1 stands for sparse
    //performs sparse optical flow
    //time(&end);
    //double total_time = double (end- start);
    //cout << "The time taken by the program is: "<< fixed << total_time << setprecision(5);
    //cout<<" seconds "<<endl;
}

pair<float,float> compute_error(string base_file, string compared_file, int total_frames){
    ifstream fin1(base_file);
    ifstream fin2(compared_file);
    int a1,a2;
    float b,c,b1,c1;
    float queue_error = 0.0;
    float dynamic_error = 0.0;
    while(fin1 >> a1)
    {
        fin1 >> b >> c;
        fin2 >> a2 >> b1 >> c1;
        queue_error+= (b-b1)*(b-b1);
        dynamic_error+= (c-c1)*(c-c1);
    }
    queue_error = (float) sqrt(queue_error)/total_frames;
    dynamic_error = (float) sqrt(dynamic_error) /total_frames;
    return {queue_error, dynamic_error};
}
/*void call_method(int number, string){
    
}
*/
