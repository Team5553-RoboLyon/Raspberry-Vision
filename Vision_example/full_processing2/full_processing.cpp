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


	//########## Threshold ##########
	cv::Mat thresholdOutput;
	cv::cvtColor(input, input, cv::COLOR_BGR2HLS);
	cv::inRange(input, cv::Scalar(0, 0, 0), cv::Scalar(255, 255, 255), thresholdOutput); //cv::inRange(input, Scalar(lowH, lowL, lowS), Scalar(highH, highL, highS), output);

	cv::imshow("threshold", thresholdOutput);


	//########## Erode and Dilate ##########
	cv::Mat openOutput;
	cv::Mat kernel = cv::getStructuringElement(cv::MORPH_CROSS, cv::Size(3, 3), cv::Point(-1, -1));
	cv::morphologyEx(thresholdOutput, openOutput, cv::MORPH_OPEN, kernel);

	cv::imshow("erode and dilate", openOutput);


	//########## Find Contours ##########
	cv::Mat findContoursOutput = openOutput.clone();
	std::vector<std::vector<cv::Point> > contours;
	cv::findContours(openOutput, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
	cv::cvtColor(findContoursOutput, findContoursOutput, cv::COLOR_GRAY2RGB);
	cv::drawContours(findContoursOutput, contours, -1, cv::Scalar(0, 0, 255), 3);

	cv::imshow("finded contours", findContoursOutput);


	//########## Approx Contours ##########
	cv::Mat approxContoursOutput = openOutput.clone();
	std::vector<std::vector<cv::Point> > approxContours;
	approxContours.resize(contours.size());
	for (size_t i = 0; i < contours.size(); i++)
	{
		cv::approxPolyDP(contours[i], approxContours[i], 3, true);
	}
	cv::cvtColor(approxContoursOutput, approxContoursOutput, cv::COLOR_GRAY2RGB);
	for (int i = 0; i < approxContours.size(); i++)
	{
		for (int n = 0; n < approxContours[i].size(); n++)
		{
			cv::putText(approxContoursOutput, std::to_string(n), approxContours[i][n], cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(255, 0, 255));
		}
	}
	cv::drawContours(approxContoursOutput, approxContours, -1, cv::Scalar(0, 0, 255), 3);

	cv::imshow("approx contours", approxContoursOutput);


	//########## Filter Contours ##########
	cv::Mat filterContoursOutput = openOutput.clone();
	std::vector<std::vector<cv::Point> > filterContours;
	for (size_t i = 0; i < approxContours.size(); i++)
	{
		cv::Rect boundRect = cv::boundingRect(approxContours[i]);

		double contourArea = cv::contourArea(approxContours[i]);
		//if (contourArea > maxArea || contourArea < minArea)
		//	continue;

		double ratio = (double)boundRect.width / boundRect.height;
		//if (contourArea > maxArea || contourArea < minArea)
		//	continue;

		if (approxContours[i].size() != 4)
			continue;

		filterContours.push_back(approxContours[i]);
	}
	cv::cvtColor(filterContoursOutput, filterContoursOutput, cv::COLOR_GRAY2RGB);
	cv::drawContours(filterContoursOutput, filterContours, -1, cv::Scalar(255, 0, 0), 3);

	cv::imshow("filtered contours", filterContoursOutput);
}

