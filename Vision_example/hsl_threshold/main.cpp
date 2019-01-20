#include "opencv2/opencv.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/videoio/videoio.hpp"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

using namespace cv;


int main(int argc, char** argv)
{
	VideoCapture cap(0);

	if (!cap.isOpened())  // check if we succeeded
		return -1;

	std::string window_name = "Threshold",
		hmin_trackbar_name = "Hue Min", hmax_trackbar_name = "Hue Max",
		smin_trackbar_name = "Saturation Min", smax_trackbar_name = "Saturation Max",
		lmin_trackbar_name = "Lightness Min", lmax_trackbar_name = "Lightness Max";

	int brightness, exposition;
	int hue[2]; 
	int lum[2];
	int sat[2];
	int max = 255;
	
	cv::namedWindow(window_name, cv::WINDOW_AUTOSIZE);
	createTrackbar("brightness", window_name, &brightness, max);
	createTrackbar("exposition", window_name, &exposition, max);
	
	createTrackbar(hmin_trackbar_name, window_name, &hue[0], max);				setTrackbarPos(hmin_trackbar_name, window_name, 0);
	createTrackbar(hmax_trackbar_name, window_name, &hue[1], max);				setTrackbarPos(hmax_trackbar_name, window_name, max);

	createTrackbar(smin_trackbar_name, window_name, &lum[0], max);				setTrackbarPos(smin_trackbar_name, window_name, 0);
	createTrackbar(smax_trackbar_name, window_name, &lum[1], max);				setTrackbarPos(smax_trackbar_name, window_name, max);

	createTrackbar(lmin_trackbar_name, window_name, &sat[0], max);				setTrackbarPos(lmin_trackbar_name, window_name, 0);
	createTrackbar(lmax_trackbar_name, window_name, &sat[1], max);				setTrackbarPos(lmax_trackbar_name, window_name, max);

	Mat input, output;
	while (true)
	{
		cap.set(CV_CAP_PROP_BRIGHTNESS, brightness);
		cap.set(CV_CAP_PROP_EXPOSURE, exposition);
		
		cap >> input; // get a new frame from camera
		imshow("input", input);

		cv::cvtColor(input, output, cv::COLOR_BGR2HLS);
		cv::inRange(output, cv::Scalar(hue[0], lum[0], sat[0]), cv::Scalar(hue[1], lum[1], sat[1]), output);
		imshow("output", output);

		if (waitKey(30) >= 0) break;
	}
	// the camera will be deinitialized automatically in VideoCapture destructor
	return 0;
}
