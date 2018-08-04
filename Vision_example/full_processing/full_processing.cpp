#include <cscore.h>
#include <ntcore.h>
#include <networktables/NetworkTableInstance.h>
#include <opencv2/core/core.hpp>
#include <opencv2/videoio/videoio.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <inttypes.h>

#include <chrono>
#include <thread>

#include "pipeline/Contour.h"

#ifndef NULL
#define NULL 0
#endif

int main()
{
	rbl::Contour *p_contour = new rbl::Contour();
	
	const char* p_outputVideoFilePath = "output.avi";

	/* Connect NetworkTables */
	/* Note:  actual IP address should be robot IP address */
	nt::NetworkTableInstance inst = nt::NetworkTableInstance::GetDefault();
	inst.StartClient("192.168.0.113");

	/* Open connection to USB Camera (video device 0 [/dev/video0]) */
	cs::UsbCamera camera("usbcam", 0);

	/* Configure Camera */
	/* Note:  Higher resolution & framerate is possible, depending upon processing cpu usage */
	double width = 800;
	double height = 600;
	int frames_per_sec = 15;
	double fov = 68.5; //Angle de vue diagonal de la Microsoft Lifecam HD-3000
	double fov_rad = fov * (M_PI/180);
	double distance_focale = width / (2*tan(fov_rad/2));
	
	camera.SetVideoMode(cs::VideoMode::PixelFormat::kMJPEG, width, height, frames_per_sec);
	camera.SetBrightness (50);
 	camera.SetWhiteBalanceAuto ();
 	//camera.SetExposureManual (60);
 	camera.SetExposureAuto ();
		
	/* Start raw Video Streaming Server */
	cs::MjpegServer rawVideoServer("raw_video_server", 8081);
	rawVideoServer.SetSource(camera);
	cs::CvSink cvsink("cvsink");
	cvsink.SetSource(camera);

	/* Start processed Video server */
	cs::CvSource cvsource("cvsource",
	cs::VideoMode::PixelFormat::kMJPEG, width, height, frames_per_sec);
	cs::MjpegServer processedVideoServer("processed_video_server", 8082);
	processedVideoServer.SetSource(cvsource);

	/* Create Video Writer, if enabled */
	cv::Size frameSize(width, height);
	cv::VideoWriter *p_videoWriter = NULL;
	if (p_outputVideoFilePath != NULL)
	{
		p_videoWriter = new cv::VideoWriter(p_outputVideoFilePath,
		cv::VideoWriter::fourcc('F', 'M', 'P', '4'), (double)frames_per_sec, frameSize, true);
	}

	/* Pre-allocate a video frame */
	cv::Mat frame;

	for (int count = 0; count < 100; count++)
	{
		/* Acquire new video frame */
		std::string videoTimestampString;
		uint64_t video_timestamp = cvsink.GrabFrame(frame);
		if (video_timestamp == 0)
		{
			std::string error_string = cvsink.GetError();
			printf("Error Grabbing Video Frame:  %s\n", error_string.c_str());
			std::this_thread::sleep_for(std::chrono::milliseconds((1000/frames_per_sec)/2));
			continue;
		}
		else
		{
			videoTimestampString = std::to_string(video_timestamp);
		}

		/* Update Network Tables with timestamps & orientation data */
		inst.GetEntry("/vision/videoOSTimestamp").SetDouble(video_timestamp);
		
		/* Invoke processing pipeline, if one is present */
		if (p_contour != NULL)
		{
			p_contour->Process(frame);
			std::vector<double> x = p_contour->GetX();
			std::vector<double> y = p_contour->GetY();
			std::vector<double> hauteur = p_contour->GetHeight();
			std::vector<double> largeur = p_contour->GetWidth();
			
			cv::drawContours(frame, p_contour->GetContours(), -54, cv::Scalar(0,255,0), 1);
			
			if(x.size() == 0)
			{
				std::cout << "Aucune cible détéctée" << std::endl;
			}
			else
			{
				for(unsigned int i = 0; i < x.size(); i++)
				{
					double angle_rad = atan((x[i]- (width/2)) / distance_focale);
					double angle = angle_rad * (180/M_PI);
					
					std::cout << "X: " << x[i] << "		" << "Y: " << y[i] << "		" << "Angle: " << angle << std::endl;
					
					std::string affichage_x = "X: " + std::to_string(x[i]);
					std::string affichage_y = "Y: " + std::to_string(y[i]);
					std::string affichage_angle = "Angle: " + std::to_string(angle);
					cv::Point point_x(10,20);
					cv::Point point_y(10,40);
					cv::Point point_angle(10,60);
					cv::putText(frame, affichage_x, point_x, cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255,0,0));
					cv::putText(frame, affichage_y, point_y, cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255,0,0));
					cv::putText(frame, affichage_angle, point_angle, cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255,0,0));
					
					cv::rectangle(frame, p_contour->GetBoundingRectangle()[i], cv::Scalar(255,0,0), 3);
				}
			}
		}

		/* Write Frame to video */
		if (p_videoWriter != NULL)
		{
			p_videoWriter->write(frame);
		}
		
		cvsource.PutFrame(frame);
	}
	
	if (p_videoWriter != NULL) {
		delete p_videoWriter;
	}
	if (p_contour != NULL) {
		delete p_contour;
	}
}

