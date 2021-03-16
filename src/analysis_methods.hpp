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
    density_calculator(video_filename, homography, crop_coordinates, frame_empty, BASE_SKIP, 0, total_frames, "out_0");
}

void method4(string video_filename, Mat homography, Rect crop_coordinates, Mat frame_empty, int total_frames, int num_threads)
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
        threads.push_back(thread(density_calculator, video_filename, homography, crop_coord[i], frame_empty(Rect(0, y_coord, crop_coord[i].width, crop_coord[i].height)), BASE_SKIP, 0, total_frames, "out_4_" + to_string(i)));
    }
    for (int i = 0; i < num_threads; i++)
        threads[i].join();

    ofstream fout("./results/out_4.txt");
    vector<ifstream> fin;
    int total_pixels = 0;
    for (int i = 0; i < num_threads; i++)
    {
        fin.push_back(ifstream("./results/out_4_" + to_string(i) + ".txt"));
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

void method5(string video_filename, Mat homography, Rect crop_coordinates, Mat frame_empty, int total_frames, int num_threads)
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
        threads.push_back(thread(density_calculator, video_filename, homography, crop_coordinates, frame_empty, BASE_SKIP, frame_limits[i], frame_limits[i + 1], "out_5_" + to_string(i)));
    }
    for (int i = 0; i < num_threads; i++)
        threads[i].join();

    ofstream fout("./results/out_5.txt");
    for (int i = 0; i < num_threads; i++)
    {
        ifstream fin("./results/out_5_" + to_string(i) + ".txt");
        string line;
        while (getline(fin, line))
        {
            fout << line << endl;
        }
        fin.close();
    }
}