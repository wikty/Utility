#include "stdafx.h"

#include <opencv2/opencv.hpp>

using namespace cv;

int main()
{
	Mat img = imread("C:\\0c55e3ecc22ba290172c937cb2e399ac21c08b03.jpg");
	imshow("Loading image", img);
	waitKey(6000);
}