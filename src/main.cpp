#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;


int main(int argc, char* argv[])
{
    Mat im_src = imread("./assets/empty.jpg");
    imshow("Empty", im_src);
    waitKey(0);
	return 0;
}