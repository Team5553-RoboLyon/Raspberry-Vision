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
	rbl::Contour p_contour;

	/* Connect NetworkTables */
	/* Note:  actual IP address should be robot IP address */
	nt::NetworkTableInstance inst = nt::NetworkTableInstance::GetDefault();
	inst.StartClient("192.168.0.113");

	/* Open connection to USB Camera (video device 0 [/dev/video0]) */
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
	cs::CvSource cvsource("cvsource", cs::VideoMode::PixelFormat::kMJPEG, width, height, frames_per_sec);
	cs::MjpegServer processedVideoServer("processed_video_server", 8082);
	processedVideoServer.SetSource(cvsource);

	/* Create Video Writer, if enabled */
	cv::VideoWriter p_videoWriter("output.avi", cv::VideoWriter::fourcc('F', 'M', 'P', '4'), (double)frames_per_sec, cv::Size(width, height), true);
	

	for (int count = 0; count < 100; count++)
	{
		/* Acquire new video frame */
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

		/* Update Network Tables with timestamps & orientation data */
		//inst.GetEntry("/vision/videoOSTimestamp").SetDouble(video_timestamp);
		
		/* Invoke processing pipeline*/
		p_contour.Process(frame);
		unsigned int nombre_de_contours = p_contour.GetNumberOfContours();
		std::vector<double> x = p_contour.GetX();
		std::vector<double> y = p_contour.GetY();
		std::vector<double> hauteur = p_contour.GetHeight();
		std::vector<double> largeur = p_contour.GetWidth();
		
		//Draw all contours
		cv::drawContours(frame, p_contour.GetContours(), -54, cv::Scalar(0,255,0), 1);
		
		if(nombre_de_contours == 0)
		{
			std::cout << "Aucune cible détéctée" << std::endl;
		}
		else
		{
			for(unsigned int i = 0; i < nombre_de_contours; i++)
			{
				//Calcul de l'angle de la cible par rapport au centre de la camera
				double angle_rad = atan((x[i]- (width/2)) / distance_focale);
				double angle = angle_rad * (180/M_PI);
				
				//Affichage des données en console
				std::cout << "X: " << x[i] << "		" << "Y: " << y[i] << "		" << "Angle: " << angle << std::endl;
				
				//Affichage des données sur l'image
				std::string affichage_x = "X: " + std::to_string(x[i]);
				std::string affichage_y = "Y: " + std::to_string(y[i]);
				std::string affichage_a = "Angle: " + std::to_string(angle);
				
				cv::putText(frame, affichage_x, cv::Point(10,20 + i*60), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255,0,0));
				cv::putText(frame, affichage_y, cv::Point(10,40 + i*60), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255,0,0));
				cv::putText(frame, affichage_a, cv::Point(10,60 + i*60), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255,0,0));
				
				//Affichage du rectangle qui entoure le contour sur l'image
				cv::rectangle(frame, p_contour.GetBoundingRectangle()[i], cv::Scalar(255,0,0), 3);
			}
		}

		/* Write Frame to video */
		p_videoWriter.write(frame);
		cvsource.PutFrame(frame);
	}

}

