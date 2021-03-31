#include "density_calculator.hpp"
#include <pthread.h>
#include <thread>
#define BASE_SKIP 3
using namespace std;
using namespace cv;

string RESULTS_DIR = "../analysis/results/";

struct density_calc_args
{
    string video_filename;
    Mat homography;
    Rect crop_coordinates;
    Mat frame_empty;
    int skip;
    int start_frame;
    int end_frame;
    string out_filename;
    int width;
    int height;
    bool sparse;
};

void *call_density_calculator(void *arguments)
{
    struct density_calc_args *args = (struct density_calc_args *)arguments;
    // cout << args->video_filename << endl;
    // cout << args->homography.size() << endl;
    density_calculator(args->video_filename, args->homography, args->crop_coordinates, args->frame_empty, args->skip, args->start_frame, args->end_frame, args->out_filename, args->width, args->height, args->sparse);
    return NULL;
}

void method0(string video_filename, string output_file, vector<Point2f> pts_src, vector<Point2f> pts_dst, Mat frame_empty, int total_frames)
{
    Mat homography = findHomography(pts_src, pts_dst);
    Rect crop_coordinates = Rect(pts_dst[0].x, pts_dst[0].y, pts_dst[2].x - pts_dst[1].x, pts_dst[1].y - pts_dst[0].y);

    warpPerspective(frame_empty, frame_empty, homography, frame_empty.size()); //warping,cropping the background frame
    frame_empty = frame_empty(crop_coordinates);

    density_calculator(video_filename, homography, crop_coordinates, frame_empty, BASE_SKIP, 0, total_frames, output_file, 1920, 1088, 0);
}

void method1(string video_filename, string output_file, vector<Point2f> pts_src, vector<Point2f> pts_dst, Mat frame_empty, int total_frames, int skip_factor)
{
    Mat homography = findHomography(pts_src, pts_dst);
    Rect crop_coordinates = Rect(pts_dst[0].x, pts_dst[0].y, pts_dst[2].x - pts_dst[1].x, pts_dst[1].y - pts_dst[0].y);

    warpPerspective(frame_empty, frame_empty, homography, frame_empty.size()); //warping,cropping the background frame
    frame_empty = frame_empty(crop_coordinates);

    //process frame N, then N+skip_frames ..
    density_calculator(video_filename, homography, crop_coordinates, frame_empty, skip_factor * BASE_SKIP, 0, total_frames, RESULTS_DIR + "out_1_skipped.txt", 1920, 1088, 0);
    //for intermediate frames use value of N
    ofstream fout(output_file);
    ifstream fin(RESULTS_DIR + "out_1_skipped.txt");
    string line;
    getline(fin, line);
    int a;
    double b, c;
    fout << "frame_number static_density dynamic_density" << endl;
    while (fin >> a)
    {
        fin >> b >> c;
        for (int i = a; i <= min(a + BASE_SKIP * skip_factor - 1, total_frames); i += BASE_SKIP)
            fout << i << " " << b << " " << c << endl;
    }
    fin.close();
}

void method2(string video_filename, string output_file, vector<Point2f> pts_src, vector<Point2f> pts_dst, Mat frame_empty, int total_frames, int width, int height)
{

    for (int i = 0; i < 4; i++)
    {
        pts_src[i].x = ((pts_src[i].x) * width) / 1920;
        pts_src[i].y = ((pts_src[i].y) * height) / 1088;
        pts_dst[i].x = ((pts_dst[i].x) * width) / 1920;
        pts_dst[i].y = ((pts_dst[i].y) * height) / 1088;
    }
    Mat homography = findHomography(pts_src, pts_dst);
    Rect crop_coordinates = Rect(pts_dst[0].x, pts_dst[0].y, pts_dst[2].x - pts_dst[1].x, pts_dst[1].y - pts_dst[0].y);
    Mat frame_empty_warped;
    warpPerspective(frame_empty, frame_empty_warped, homography, frame_empty.size()); //warping,cropping the background frame
    Mat frame_empty_cropped = frame_empty(crop_coordinates);

    density_calculator(video_filename, homography, crop_coordinates, frame_empty_cropped, BASE_SKIP, 0, total_frames, output_file, width, height, 0);
}

void method3(string video_filename, string output_file, vector<Point2f> pts_src, vector<Point2f> pts_dst, Mat frame_empty, int total_frames, int num_threads)
{
    Mat homography = findHomography(pts_src, pts_dst);
    Rect crop_coordinates = Rect(pts_dst[0].x, pts_dst[0].y, pts_dst[2].x - pts_dst[1].x, pts_dst[1].y - pts_dst[0].y);
    warpPerspective(frame_empty, frame_empty, homography, frame_empty.size()); //warping,cropping the background frame
    frame_empty = frame_empty(crop_coordinates);
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
    vector<pthread_t> threads(num_threads);
    int y_coord;
    struct density_calc_args args[num_threads];
    for (int i = 0; i < num_threads; i++)
    {
        if (i == 0)
            y_coord = 0;
        else
            y_coord += crop_coord[i - 1].height;

        args[i].video_filename = video_filename;
        args[i].homography = homography;
        // cout << args.homography.type() << endl;
        args[i].crop_coordinates = crop_coord[i];
        args[i].frame_empty = frame_empty(Rect(0, y_coord, crop_coord[i].width, crop_coord[i].height));
        args[i].skip = BASE_SKIP;
        args[i].start_frame = 0;
        args[i].end_frame = total_frames;
        args[i].out_filename = RESULTS_DIR + "out_3_" + to_string(i) + ".txt";
        args[i].width = 1920;
        args[i].height = 1088;
        args[i].sparse = false;
        pthread_create(&threads[i], NULL, &call_density_calculator, &args[i]);
        // threads.push_back(thread(density_calculator, video_filename, homography, crop_coord[i], frame_empty(Rect(0, y_coord, crop_coord[i].width, crop_coord[i].height)), BASE_SKIP, 0, total_frames, "out_3_" + to_string(i), 1920, 1088, 0));
    }
    for (int i = 0; i < num_threads; i++)
    {
        pthread_join(threads[i], NULL);
    }
    ofstream fout(output_file);
    vector<ifstream> fin;
    int total_pixels = 0;
    string line;
    for (int i = 0; i < num_threads; i++)
    {
        fin.push_back(ifstream(RESULTS_DIR + "out_3_" + to_string(i) + ".txt"));
        total_pixels += pixels[i];
        getline(fin[i], line);
    }
    int a;
    double b, c;
    fout << "frame_number static_density dynamic_density" << endl;
    while (fin[0] >> a)
    {
        fin[0] >> b >> c;

        double b_acc = b * pixels[0], c_acc = c * pixels[0];

        for (int i = 1; i < num_threads; i++)
        {
            fin[i] >> a >> b >> c;
            b_acc = b_acc + b * pixels[i];
            c_acc = c_acc + c * pixels[i];
        }
        fout << a << " " << (b_acc / total_pixels) << " " << (c_acc / total_pixels) << endl;
    }
}

void method4(string video_filename, string output_file, vector<Point2f> pts_src, vector<Point2f> pts_dst, Mat frame_empty, int total_frames, int num_threads)
{
    Mat homography = findHomography(pts_src, pts_dst);
    Rect crop_coordinates = Rect(pts_dst[0].x, pts_dst[0].y, pts_dst[2].x - pts_dst[1].x, pts_dst[1].y - pts_dst[0].y);

    warpPerspective(frame_empty, frame_empty, homography, frame_empty.size()); //warping,cropping the background frame
    frame_empty = frame_empty(crop_coordinates);

    int frame_limits[num_threads + 1];
    for (int i = 0; i <= num_threads; i++)
    {
        frame_limits[i] = (total_frames * i) / num_threads;
    }

    vector<pthread_t> threads(num_threads);
    struct density_calc_args args[num_threads];

    for (int i = 0; i < num_threads; i++)
    {
        args[i].video_filename = video_filename;
        args[i].homography = homography;
        args[i].crop_coordinates = crop_coordinates;
        args[i].frame_empty = frame_empty;
        args[i].skip = BASE_SKIP;
        args[i].start_frame = frame_limits[i];
        args[i].end_frame = frame_limits[i + 1];
        args[i].out_filename = RESULTS_DIR + "out_4_" + to_string(i) + ".txt";
        args[i].width = 1920;
        args[i].height = 1088;
        args[i].sparse = false;
        pthread_create(&threads[i], NULL, &call_density_calculator, &args[i]);
    }
    for (int i = 0; i < num_threads; i++)
    {
        pthread_join(threads[i], NULL);
    }
    ofstream fout(output_file);
    fout << "frame_number static_density dynamic_density" << endl;
    for (int i = 0; i < num_threads; i++)
    {
        ifstream fin;
        fin.open(RESULTS_DIR + "out_4_" + to_string(i) + ".txt");
        string line;
        getline(fin, line);
        while (getline(fin, line))
        {
            fout << line << endl;
        }
        fin.close();
    }
}

void bonus_method(string video_filename, string output_file, vector<Point2f> pts_src, vector<Point2f> pts_dst, Mat frame_empty, int total_frames, bool sparse)
{
    Mat homography = findHomography(pts_src, pts_dst);
    Rect crop_coordinates = Rect(pts_dst[0].x, pts_dst[0].y, pts_dst[2].x - pts_dst[1].x, pts_dst[1].y - pts_dst[0].y);

    warpPerspective(frame_empty, frame_empty, homography, frame_empty.size()); //warping,cropping the background frame
    frame_empty = frame_empty(crop_coordinates);

    //time_t start,end;
    //time(&start);
    if (!sparse)
    {
        density_calculator(video_filename, homography, crop_coordinates, frame_empty, BASE_SKIP, 0, total_frames, output_file, 1920, 1088, 0);
    }
    else
    {
        density_calculator(video_filename, homography, crop_coordinates, frame_empty, BASE_SKIP, 0, total_frames, output_file, 1920, 1088, 1);
    }
    // last parameter is a boolean, 1 stands for sparse
    //performs sparse optical flow
    //time(&end);
    //double total_time = double (end- start);
    //cout << "The time taken by the program is: "<< fixed << total_time << setprecision(5);
    //cout<<" seconds "<<endl;
}

pair<double, double> compute_error(string base_file, string compared_file, int total_frames)
{

    ifstream fin1(base_file);
    string line;
    getline(fin1, line);
    ifstream fin2(compared_file);
    getline(fin2, line);
    int a1, a2;
    double b, c, b1, c1;
    double queue_error = 0.0;
    double dynamic_error = 0.0;
    while (fin1 >> a1)
    {
        fin1 >> b >> c;
        fin2 >> a2 >> b1 >> c1;
        queue_error += (b - b1) * (b - b1);
        dynamic_error += (c - c1) * (c - c1);
    }
    double total_processed = (double)total_frames / BASE_SKIP;
    queue_error = (double)sqrt((double)(queue_error) / total_processed);
    dynamic_error = (double)sqrt((double)(dynamic_error) / total_processed);
    return {queue_error, dynamic_error};
}

void call_method(int method_number, string video_filename, string output_file, vector<Point2f> pts_src, vector<Point2f> pts_dst, Mat frame_empty, int total_frames, ofstream &utility_runtime, int method_arg1 = 0, int method_arg2 = 0)
{
    time_t start, end;
    time(&start);
    switch (method_number)
    {
    case 0:
        method0(video_filename, output_file, pts_src, pts_dst, frame_empty, total_frames);
        break;
    case 1:
        method1(video_filename, output_file, pts_src, pts_dst, frame_empty, total_frames, method_arg1);
        break;
    case 2:
        method2(video_filename, output_file, pts_src, pts_dst, frame_empty, total_frames, method_arg1, method_arg2);
        break;
    case 3:
        method3(video_filename, output_file, pts_src, pts_dst, frame_empty, total_frames, method_arg1);
        break;
    case 4:
        method4(video_filename, output_file, pts_src, pts_dst, frame_empty, total_frames, method_arg1);
        break;
    case 5:
        bonus_method(video_filename, output_file, pts_src, pts_dst, frame_empty, total_frames, method_arg1);
        break;
    }

    time(&end);
    double total_time = double(end - start);
    pair<double, double> errors = compute_error(RESULTS_DIR + "method_0.txt", output_file, total_frames);
    utility_runtime << method_number << " " << method_arg1 << " " << method_arg2 << " " << errors.first << " " << errors.second << " " << total_time << endl;
}
