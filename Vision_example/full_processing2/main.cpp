#include <cscore.h>
#include <ntcore.h>
#include <networktables/NetworkTable.h>
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

double process(cv::Mat &input, bool afficherCout = true, bool afficherImages = true);

int main(int argc, char *argv[])
{
	bool quietCout = false;
	bool quietWindows = false;
	for(int i = 0; i < argc; i++)
	{
		if(strcmp(argv[i], "quietc") == 0)
			quietCout = true;
		else if(strcmp(argv[i], "quietw") == 0)
			quietWindows = true;
	}

	time_t timestamp_debut = std::time (0);
	
	auto inst = nt::NetworkTableInstance::GetDefault();
	auto table = inst.GetTable("datatable");
	auto entry = table->GetEntry("Angle");
	inst.StartClientTeam(5553);
	
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
		
		double centerX = process(frame, !quietCout, !quietWindows);
		double angle;
		if(centerX == -1)
		{
			angle = 0;
		}
		else
		{
			double angle_rad = atan((centerX- (width/2)) / distance_focale);
			angle = angle_rad * (180/M_PI);
		}
		
		if (entry.Exists())
		{
			entry.SetDouble(angle);
			if(!quietCout)
				std::cout << "Angle envoyé" << std::endl << std::endl;
	    }

		if(!quietCout)
			std::cout << "Angle " << angle << std::endl << std::endl;
		
		if (cv::waitKey(30) >= 0) break;
	}
	
	time_t timestamp_fin = std::time (0);
	std::cout << "Programme terminé au bout de " << timestamp_fin - timestamp_debut << " secondes" << std::endl;
}

double process(cv::Mat &input, bool afficherCout, bool afficherImages)
{
	if(afficherImages)
		cv::imshow("input", input);


	//########## Threshold ##########
	cv::Mat thresholdOutput;
	cv::cvtColor(input, input, cv::COLOR_BGR2HLS);
	cv::inRange(input, cv::Scalar(43, 225, 225), cv::Scalar(87, 255, 255), thresholdOutput); //cv::inRange(input, Scalar(lowH, lowL, lowS), Scalar(highH, highL, highS), output);

	if(afficherImages)
		cv::imshow("threshold", thresholdOutput);


	//########## Erode and Dilate ##########
	cv::Mat openOutput;
	cv::Mat kernel = cv::getStructuringElement(cv::MORPH_CROSS, cv::Size(3, 3), cv::Point(-1, -1));
	cv::morphologyEx(thresholdOutput, openOutput, cv::MORPH_OPEN, kernel);

	if(afficherImages)
		cv::imshow("erode and dilate", openOutput);


	//########## Find Contours ##########
	cv::Mat findContoursOutput = openOutput.clone();
	std::vector<std::vector<cv::Point> > contours;
	cv::findContours(openOutput, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
	cv::cvtColor(findContoursOutput, findContoursOutput, cv::COLOR_GRAY2RGB);
	cv::drawContours(findContoursOutput, contours, -1, cv::Scalar(0, 0, 255), 3);

	if(afficherImages)
		cv::imshow("find contours", findContoursOutput);


	//########## Approx Contours ##########
	cv::Mat approxContoursOutput = openOutput.clone();
	std::vector<std::vector<cv::Point> > approxContours;
	approxContours.resize(contours.size());
	for (size_t i = 0; i < contours.size(); i++)
	{
		cv::approxPolyDP(contours[i], approxContours[i], 17, true);
	}
	cv::cvtColor(approxContoursOutput, approxContoursOutput, cv::COLOR_GRAY2RGB);
	for (size_t i = 0; i < approxContours.size(); i++)
	{
		for (size_t n = 0; n < approxContours[i].size(); n++)
		{
			cv::putText(approxContoursOutput, std::to_string(n), approxContours[i][n], cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(255, 0, 255));
		}
	}
	cv::drawContours(approxContoursOutput, approxContours, -1, cv::Scalar(0, 0, 255), 3);

	if(afficherImages)
		cv::imshow("approx contours", approxContoursOutput);


	//########## Filter Contours ##########
	cv::Mat filterContoursOutput = openOutput.clone();
	std::vector<std::vector<cv::Point> > filterContours;
	if(afficherCout)
		std::cout << "############### " << approxContours.size() << " CONTOURS DETECTES ###############" << std::endl;
	
	for (size_t i = 0; i < approxContours.size(); i++)
	{
		//std::cout << "######## CONTOUR N " << i << " ########" << std::endl;
		cv::Rect boundRect = cv::boundingRect(approxContours[i]);

		double contourArea = cv::contourArea(approxContours[i]);
		//std::cout << "Area " << contourArea << std::endl;
		if (contourArea < 10)//(contourArea > maxArea || contourArea < minArea)
			continue;

		double ratio = (double)boundRect.width / boundRect.height;
		//std::cout << "Ratio " << ratio << std::endl;
		//if (contourArea > maxArea || contourArea < minArea)
		//	continue;
		
		//std::cout << "Angles " << approxContours[i].size() << std::endl;
		//if (approxContours[i].size() != 4)
		//	continue;

		filterContours.push_back(approxContours[i]);
	}
	if(afficherCout)
		std::cout << "###### " << filterContours.size() << " contour trouve" << std::endl;
	cv::cvtColor(filterContoursOutput, filterContoursOutput, cv::COLOR_GRAY2RGB);
	cv::drawContours(filterContoursOutput, filterContours, -1, cv::Scalar(255, 0, 0), 3);

	if(afficherImages)
		cv::imshow("filtered contours", filterContoursOutput);
	
	double centerX;
	if(filterContours.size() != 0)
	{
		cv::Rect boundRect = cv::boundingRect(filterContours[0]);
		centerX = boundRect.x + (boundRect.width / 2);
	}
	else
	{
		centerX = -1;
	}
	
	return centerX;
}

