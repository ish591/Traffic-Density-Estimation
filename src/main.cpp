#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

vector<pair<int, int>> selected_pts;

void clickEvent(int event, int x, int y, int flags, void *params)
{

    Mat *im_empty = (Mat *)params;
    if (event == EVENT_LBUTTONDOWN)
    {
        cout << "(" << x << ", " << y << ")\n";
        circle(*im_empty, Point2f(x, y), 9, Scalar(20, 20, 20), -1);
        selected_pts.push_back({x, y});
        imshow("Empty", *im_empty);
    }
}

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

vector<Point2f> get_dst_points(vector<Point2f> pts_src)
{
    vector<Point2f> pts_dst;
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

int main(int argc, char *argv[])
{
    Mat im_empty = imread("./assets/empty.jpg", IMREAD_GRAYSCALE);
    Mat im_traffic = imread("./assets/traffic.jpg", IMREAD_GRAYSCALE);
    Mat im_empty_copy = im_empty.clone();
    imshow("Empty", im_empty_copy);

    while (selected_pts.size() < 4)
    {
        setMouseCallback("Empty", clickEvent, (void *)&im_empty_copy);
        waitKey(200);
    }

    destroyWindow("Empty");
    vector<Point2f> pts_src, pts_dst;
    pts_src = get_src_points();
    pts_dst = get_dst_points(pts_src);
    waitKey(100);

    // change to calculate dynamically

    Mat homography = findHomography(pts_src, pts_dst);

    Mat im_empty_warped, im_empty_cropped, im_traffic_warped, im_traffic_cropped;

    warpPerspective(im_empty, im_empty_warped, homography, im_empty.size());
    warpPerspective(im_traffic, im_traffic_warped, homography, im_traffic.size());

    imshow("Warped", im_empty_warped);
    waitKey(0);
    destroyWindow("Warped");

    Rect crop_coordinates = Rect(pts_dst[0].x, pts_dst[0].y, pts_dst[2].x - pts_dst[1].x, pts_dst[1].y - pts_dst[0].y); // make crop coordinates dynamic
    //cout << pts_dst[0].x << " " << pts_dst[0].y << endl;
    //Rect crop_coordinates = Rect(pts_dst[0].x, pts_dst[0].y, 100, 100); // make crop coordinates dynamic
    im_empty_cropped = im_empty_warped(crop_coordinates);
    im_traffic_cropped = im_traffic_warped(crop_coordinates);

    imshow("Cropped", im_empty_cropped);
    waitKey(0);
    destroyWindow("Cropped");

    imwrite("empty_warped.jpg", im_empty_warped);
    imwrite("empty_cropped.jpg", im_empty_cropped);
    imwrite("traffic_warped.jpg", im_traffic_warped);
    imwrite("traffic_cropped.jpg", im_traffic_cropped);

    return 0;
}
