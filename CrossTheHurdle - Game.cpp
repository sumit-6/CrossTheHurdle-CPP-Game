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

int max(int a, int b)
{
	if (a > b)
	{
		return a;
	}
	return b;
}

class Data
{
protected:
	Mat img;
	queue<vector<int>> newPoints;
	vector<vector<int>> myColors;
	vector<Scalar> myColorValues;
public:
	Data(vector<int> colors[], Scalar markers[], int n)
	{
		for (int i = 0; i < n; i++)
		{
			myColors.push_back(colors[i]);
			myColorValues.push_back(markers[i]);
		}
	}

	bool Validate(queue<vector<int>> Locations, int i1, int i2, int i3, int rint1, int rint2, int rint3)
	{
		while (!Locations.empty())
		{
			vector<int> v = Locations.front();
			Locations.pop();
			int x = 640 - v[0], y = v[1];

			if ((x >= (590 - i1)) && (x <= (640 - i1)))
			{
				if (!((y >= (480 - rint1 - 40)) && (y <= (480 - rint1))))
				{
					return false;
				}
			}
			if ((x >= (590 - i2)) && (x <= (640 - i2)))
			{
				if (!((y >= (480 - rint2 - 40)) && (y <= (480 - rint2))))
				{
					return false;
				}
			}
			if ((x >= (590 - i3)) && (x <= (640 - i3)))
			{
				if (!((y >= (480 - rint3 - 40)) && (y <= (480 - rint3))))
				{
					return false;
				}
			}
		}
		return true;
	}

	int levelUp(int x)
	{
		return 30 / (2 + (3 * exp(-x)));
	}
};

class Pens : public Data
{
public:
	Pens(vector<int> colors[], Scalar markers[], int n) : Data(colors, markers, n) {}
	Point getContours(Mat imgDil)
	{
		// Getting Contours
		vector<vector<Point>> contours;
		vector<Vec4i> hierarchy;
		Point myPoint(0, 0);
		findContours(imgDil, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

		// Filtering out the variations
		double area, peri;
		vector<vector<Point>> conPoly(contours.size());
		vector<Rect> boundRect(contours.size());
		string objectType;
		for (int i = 0; i < contours.size(); i++)
		{
			area = (double)contourArea(contours[i]);
			if (area > 1000.0)
			{
				peri = arcLength(contours[i], true);
				approxPolyDP(contours[i], conPoly[i], 0.02 * peri, true);

				boundRect[i] = (boundingRect(conPoly[i]));
				myPoint.x = boundRect[i].x + boundRect[i].width / 2;
				myPoint.y = boundRect[i].y;
				drawContours(img, conPoly, i, Scalar(255, 0, 255), 1);
			}
		}
		return myPoint;
	}

	void findColor(queue<vector<int>>& myPoints)
	{
		Mat imgHSV;

		cvtColor(img, imgHSV, COLOR_BGR2HSV);
		Mat mask;
		for (int i = 0; i < myColors.size(); i++)
		{
			Scalar lower(myColors[i][0], myColors[i][1], myColors[i][2]);
			Scalar upper(myColors[i][3], myColors[i][4], myColors[i][5]);
			inRange(imgHSV, lower, upper, mask);

			Point myPoint = getContours(mask);
			if (myPoint.x != 0 && myPoint.y != 0)
			{
				newPoints.push({ myPoint.x, myPoint.y, i });
			}
		}
	}
};

class Screen : public Pens
{
public:
	Screen(vector<int> colors[], Scalar markers[], int n) : Pens(colors, markers, n) {}
	void drawOnCanvas(queue<vector<int>> myPoints, vector<Scalar> color)
	{
		while (!myPoints.empty())
		{
			vector<int> v = myPoints.front();
			myPoints.pop();
			circle(img, Point(v[0], v[1]), 4, color[v[2]], FILLED);
		}
	}

	void buildObstackles(int rint, Scalar colour, int i)
	{
		int width = 50;
		rectangle(img, Point(590 - i, 0), Point(640 - i, 480 - rint - width), colour, FILLED);
		rectangle(img, Point(590 - i, 480 - rint), Point(640 - i, 480), colour, FILLED);
	}

	void Game()
	{
	start:
		VideoCapture cap(0);
		float w = 640, h = 480;
		auto colour = Scalar(155, 0, 200);
		int i1 = 0, i2 = -300, i3 = -600;
		srand(time(0));
		int rint1 = rand() % 420;
		int rint2 = rand() % 420;
		int rint3 = rand() % 420;
		int speed = levelUp(0);
		long long int score = 0;
		int T = 0;
		int x = 0;
		Point2f source[4] = { {0.0f,0.0f}, {w,0.0f}, {0.0f, h}, {w,h} };
		Point2f destination[4] = { {w,0.0f}, {0.0f,0.0f}, {w,h}, {0.0f, h} };
		while (true)
		{
			string res = "Score : " + to_string(score);
			cap.read(img);
			findColor(newPoints);
			if (newPoints.size() > 5)
			{
				newPoints.pop();
			}
			drawOnCanvas(newPoints, myColorValues);

			auto matrix = getPerspectiveTransform(source, destination);
			warpPerspective(img, img, matrix, Point(w, h));

			if (640 - i1 > 0)
			{
				buildObstackles(rint1, colour, i1);
			}
			else
			{
				i1 = i3 - 300;
				rint1 = rand() % 450;
			}
			if (640 - i2 > 0)
			{
				buildObstackles(rint2, colour, i2);
			}
			else
			{
				i2 = i1 - 300;
				rint2 = rand() % 450;
			}
			if (640 - i3 > 0)
			{
				buildObstackles(rint3, colour, i3);
			}
			else
			{
				i3 = i2 - 300;
				rint3 = rand() % 450;
			}
			putText(img, res, Point(10, 25), FONT_HERSHEY_TRIPLEX, 1.05, Scalar(20, 255, 20), 2);
			if (!Validate(newPoints, i1, i2, i3, rint1, rint2, rint3))
			{
				rectangle(img, Point(127, 219), Point(518, 353), Scalar(34, 139, 34), FILLED);
				rectangle(img, Point(130, 222), Point(515, 350), Scalar(0, 0, 0), FILLED);
				putText(img, "You've Lost", Point(220, 262), FONT_HERSHEY_DUPLEX, 1.05, Scalar(255, 255, 255), 2);
				putText(img, "Press any key to replay", Point(150, 290), FONT_HERSHEY_DUPLEX, 0.8, Scalar(255, 255, 255), 2);
				putText(img, "Press \"Enter\" key to leave", Point(150, 320), FONT_HERSHEY_DUPLEX, 0.8, Scalar(255, 255, 255), 2);

				imshow("Image", img);
				int choice = waitKey(0);
				if (choice != 13)
				{
					goto start;
				}
				goto statement;
			}

			i1 += speed;
			i2 += speed;
			i3 += speed;
			imshow("Image", img);
			waitKey(1);
			T++;
			if (T % 20 == 0)
			{
				score++;
				if (T > 200)
				{
					x++;
					x %= MOD;
					speed = levelUp(x);
					T = 0;
				}
			}
		}
	statement:;
	}
};

int main()
{
	vector<int> redPen = { 70, 178, 88, 179, 255, 204 };
	Scalar redMarker = { 0, 0, 255 };
	vector<int> yellowPen = { 20, 133, 153, 36, 255, 255 };
	Scalar yellowMarker = { 0, 255, 255 };
	vector<int> colors[] = { redPen, yellowPen };
	Scalar markers[] = { redMarker, yellowMarker };
	int n = sizeof(colors)/sizeof(colors[0]);
	Screen screen(colors, markers, n);
	screen.Game();
	return 0;
}
