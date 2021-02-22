#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

vector<pair<int,int>> temp;
void clickEvent(int event, int x, int y, int flags, void *params)
{
    if(event == EVENT_LBUTTONDOWN)
    {
        cout << "(" << x << ", " << y << ")\n";
        temp.push_back({x,y});
    }
}


vector<Point2f> sortpoints(){
    vector<Point2f> pts_src;
    sort(temp.begin(),temp.end());
    if(temp[0].second>temp[1].second){
        pair<int,int>u=temp[0];
        temp[0]=temp[1];
        temp[1]=u;
    }
    if(temp[3].second>temp[2].second){
           pair<int,int>u=temp[2];
           temp[2]=temp[3];
           temp[3]=u;
    }
    for (int i=0;i<4;i++){
        pts_src.push_back(Point2f(temp[i].first,temp[i].second));
    }
    return pts_src;
}


int main(int argc, char* argv[])
{
    Mat im_empty = imread("./assets/empty.jpg", IMREAD_GRAYSCALE);
    Mat im_traffic = imread("./assets/traffic.jpg", IMREAD_GRAYSCALE);

    imshow("Empty", im_empty);
    
    setMouseCallback("Empty", clickEvent);

    while(temp.size() < 4)
    {
        waitKey(200);
    }

    destroyWindow("Empty");
    vector<Point2f> pts_src, pts_dst;
    pts_src=sortpoints();
    
    waitKey(100);
    
    // change to calculate dynamically
    pts_dst.push_back(Point2f(472,52));
    pts_dst.push_back(Point2f(472, 830));
    pts_dst.push_back(Point2f(800, 830));
    pts_dst.push_back(Point2f(800,52));
    
    cout<<im_empty.size()<<endl;
    Mat homography = findHomography(pts_src, pts_dst);

    Mat im_empty_warped, im_empty_cropped, im_traffic_warped, im_traffic_cropped;
    
    warpPerspective(im_empty, im_empty_warped, homography, im_empty.size());
    warpPerspective(im_traffic, im_traffic_warped, homography, im_traffic.size());

    imshow("Warped", im_empty_warped);
    waitKey(0);
    destroyWindow("Warped");

    Rect crop_coordinates = Rect(472, 52, 328, 778); // make crop coordinates dynamic
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

