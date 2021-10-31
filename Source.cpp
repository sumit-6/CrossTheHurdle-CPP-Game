#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include <queue>
#include <string>
#include <opencv2/objdetect.hpp>
const int MOD = 1e9 + 7;

using namespace cv;
using namespace std;

void colorDetection()
{
	VideoCapture cap(0);
	Mat img;

	Mat imgHSV, mask;
	//resize(img, img, Size(1000, 500));

	int hmin = 0, smin = 0, vmin = 0;
	int hmax = 179, smax = 255, vmax = 255;
	//cvtColor(img, imgHSV, COLOR_BGR2HSV);

	namedWindow("TrackBar", (640, 200));
	createTrackbar("Hue Min", "TrackBar", &hmin, 179);
	createTrackbar("Hue Max", "TrackBar", &hmax, 179);
	createTrackbar("Saturation Min", "TrackBar", &smin, 255);
	createTrackbar("Saturation Max", "TrackBar", &smax, 255);
	createTrackbar("Value Min", "TrackBar", &vmin, 255);
	createTrackbar("Value Max", "TrackBar", &vmax, 255);

	while (true)
	{
		cap.read(img);
		cvtColor(img, imgHSV, COLOR_BGR2HSV);
		Scalar lower(hmin, smin, vmin);
		Scalar upper(hmax, smax, vmax);
		inRange(imgHSV, lower, upper, mask);

		imshow("Image", img);
		//imshow("Image HSV", imgHSV);
		imshow("Image mask", mask);
		waitKey(10);

	}
}

int main()
{
	colorDetection();
	return 0;
}
