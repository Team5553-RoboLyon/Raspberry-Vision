#include <cscore.h>
#include <ntcore.h>
#include <networktables/NetworkTableInstance.h>
#include <opencv2/core/core.hpp>
#include <opencv2/videoio/videoio.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/opencv.hpp"
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <inttypes.h>

#include <chrono>
#include <thread>

#ifndef NULL
#define NULL 0
#endif

void process(cv::Mat &input);
void hslThreshold(cv::Mat &input, cv::Mat &out, double hue[], double sat[], double lum[]);
void findContours(cv::Mat &input, std::vector<std::vector<cv::Point> > &contours, bool externalOnly = false);
void filterContours(std::vector<std::vector<cv::Point> > &inputContours, std::vector<std::vector<cv::Point> > &output, double minArea, double minPerimeter, double minWidth, double maxWidth, double minHeight, double maxHeight, double solidity[], double maxVertexCount, double minVertexCount, double minRatio, double maxRatio);

int main()
{
	time_t timestamp_debut = std::time (0);
	
	cs::UsbCamera camera("usbcam", 0);

	/* Configure Camera */
	/* Note:  Higher resolution & framerate is possible, depending upon processing cpu usage */
	const double width = 800;
	const double height = 600;
	const int frames_per_sec = 15;
	const double fov = 68.5; //Angle de vue diagonal de la Microsoft Lifecam HD-3000
	const double fov_rad = fov * (M_PI/180);
	const double distance_focale = width / (2*tan(fov_rad/2));
	
	camera.SetVideoMode(cs::VideoMode::PixelFormat::kMJPEG, width, height, frames_per_sec);
	camera.SetBrightness (0);
	
	cs::CvSink cvsink("cvsink");
	cvsink.SetSource(camera);
	
	for (int count = 0; count < 100; count++)
	{
		cv::Mat frame;
		uint64_t video_timestamp = cvsink.GrabFrame(frame);
		if (video_timestamp == 0)
		{
			std::cout << "Error Grabbing Video Frame:	" << cvsink.GetError() << std::endl;
			std::this_thread::sleep_for(std::chrono::milliseconds((1000/frames_per_sec)/2));
			continue;
		}
		else
		{
			std::string videoTimestampString = std::to_string(video_timestamp);
		}
		
		process(frame);
		if (cv::waitKey(30) >= 0) break;
	}
	
	time_t timestamp_fin = std::time (0);
	std::cout << "Programme terminé au bout de " << timestamp_fin - timestamp_debut << " secondes" << std::endl;
}

void process(cv::Mat &input)
{
	cv::imshow("input", input);
	
	cv::Mat hslThresholdOutput;
	double Hue[] = {123.0, 152.0};
	double Saturation[] = {185.0, 255.0};
	double Luminance[] = {255.0, 255.0};
	hslThreshold(input, hslThresholdOutput, Hue, Saturation, Luminance);
	cv::imshow("output", hslThresholdOutput);
	
	std::vector<std::vector<cv::Point> > findContoursOutput;
	findContours(hslThresholdOutput, findContoursOutput);
	
	std::vector<std::vector<cv::Point> > filterContoursOutput;
	double filterContoursMinArea = 60.0;  // default Double
	double filterContoursMinPerimeter = 0;  // default Double
	double filterContoursMinWidth = 0;  // default Double
	double filterContoursMaxWidth = 1000;  // default Double
	double filterContoursMinHeight = 0;  // default Double
	double filterContoursMaxHeight = 1000;  // default Double
	double filterContoursSolidity[] = {0, 100};
	double filterContoursMaxVertices = 1000000;  // default Double
	double filterContoursMinVertices = 0;  // default Double
	double filterContoursMinRatio = 0;  // default Double
	double filterContoursMaxRatio = 1000;  // default Double
	filterContours(findContoursOutput, filterContoursOutput, filterContoursMinArea, filterContoursMinPerimeter, filterContoursMinWidth, filterContoursMaxWidth, filterContoursMinHeight, filterContoursMaxHeight, filterContoursSolidity, filterContoursMaxVertices, filterContoursMinVertices, filterContoursMinRatio, filterContoursMaxRatio);
}

void hslThreshold(cv::Mat &input, cv::Mat &out, double hue[], double sat[], double lum[])
{
	cv::cvtColor(input, out, cv::COLOR_BGR2HLS);
	cv::inRange(out, cv::Scalar(hue[0], lum[0], sat[0]), cv::Scalar(hue[1], lum[1], sat[1]), out);
}

void findContours(cv::Mat &input, std::vector<std::vector<cv::Point> > &contours, bool externalOnly)
{
	std::vector<cv::Vec4i> hierarchy;
	contours.clear();
	int mode = externalOnly ? cv::RETR_EXTERNAL : cv::RETR_LIST;
	int method = cv::CHAIN_APPROX_SIMPLE;
	cv::findContours(input, contours, hierarchy, mode, method);
}

void filterContours(std::vector<std::vector<cv::Point> > &inputContours, std::vector<std::vector<cv::Point> > &output, double minArea, double minPerimeter, double minWidth, double maxWidth, double minHeight, double maxHeight, double solidity[], double maxVertexCount, double minVertexCount, double minRatio, double maxRatio)
{
	std::vector<cv::Point> hull;
	output.clear();
	for (std::vector<cv::Point> contour: inputContours)
	{
		cv::Rect bb = boundingRect(contour);
		if (bb.width < minWidth || bb.width > maxWidth) continue;
		if (bb.height < minHeight || bb.height > maxHeight) continue;
		double area = cv::contourArea(contour);
		if (area < minArea) continue;
		if (arcLength(contour, true) < minPerimeter) continue;
		cv::convexHull(cv::Mat(contour, true), hull);
		double solid = 100 * area / cv::contourArea(hull);
		if (solid < solidity[0] || solid > solidity[1]) continue;
		if (contour.size() < minVertexCount || contour.size() > maxVertexCount)	continue;
		double ratio = (double) bb.width / (double) bb.height;
		if (ratio < minRatio || ratio > maxRatio) continue;
		output.push_back(contour);
	}
}
