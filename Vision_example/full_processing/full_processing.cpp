#include <cscore.h>
#include <ntcore.h>
#include <networktables/NetworkTableInstance.h>
#include <opencv2/core/core.hpp>
#include <opencv2/videoio/videoio.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <inttypes.h>

#include <chrono>
#include <thread>

#include "pipeline/Contour.h"

using namespace cs;
using namespace nt;
using namespace cv;
using namespace grip;

#ifndef NULL
#define NULL 0
#endif

int main() {
	/* Instantiate pipeline exported from GRIP.  If not using grip, set pipeline = NULL; */
	Contour *p_contour = new Contour();
	
	const char* p_outputVideoFilePath = "output.avi"; /* Set to NULL if video writing not desired */

	/* Connect NetworkTables */
	/* Note:  actual IP address should be robot IP address */
	NetworkTableInstance inst = NetworkTableInstance::GetDefault();
	inst.StartClient("192.168.0.113");

	/* Open connection to USB Camera (video device 0 [/dev/video0]) */
	UsbCamera camera("usbcam", 0);

	/* Configure Camera */
	/* Note:  Higher resolution & framerate is possible, depending upon processing cpu usage */
	double width = 320;
	double height = 240;
	int frames_per_sec = 15;
	camera.SetVideoMode(VideoMode::PixelFormat::kMJPEG, width, height, frames_per_sec);
		
	/* Start raw Video Streaming Server */
	MjpegServer rawVideoServer("raw_video_server", 8081);
	rawVideoServer.SetSource(camera);
	CvSink cvsink("cvsink");
	cvsink.SetSource(camera);

	/* Start processed Video server */
	CvSource cvsource("cvsource",
	VideoMode::PixelFormat::kMJPEG, width, height, frames_per_sec);
	MjpegServer processedVideoServer("processed_video_server", 8082);
	processedVideoServer.SetSource(cvsource);

	/* Create Video Writer, if enabled */
	Size frameSize(width, height);
	VideoWriter *p_videoWriter = NULL;
	if (p_outputVideoFilePath != NULL)
	{
		p_videoWriter = new VideoWriter(p_outputVideoFilePath,
		VideoWriter::fourcc('F', 'M', 'P', '4'), (double)frames_per_sec, frameSize, true);
	}

	/* Pre-allocate a video frame */
	Mat frame;

	int count = 0;
	while (count < 200) {
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
			//printf("Video Timestamp:  %llu\n", video_timestamp);
		}

		/* Update Network Tables with timestamps & orientation data */
		inst.GetEntry("/vmx/videoOSTimestamp").SetDouble(video_timestamp);

		/* Invoke processing pipeline, if one is present */
		if (p_contour != NULL)
		{
		p_contour->Process(frame);
		std::cout << "X: " << p_contour->GetX() << std::endl;
		std::cout << "Y: " << p_contour->GetY() << std::endl;
		
		cv::Point Top_left_corner(p_contour->GetX(), p_contour->GetY());
		cv::Point Opposite_corner(p_contour->GetX()+p_contour->GetWidth(), p_contour->GetY()+p_contour->GetHeight());
		rectangle(frame, Top_left_corner, Opposite_corner,  Scalar(255,0,0));
		}


		/* Write Frame to video */
		if (p_videoWriter != NULL) {
			p_videoWriter->write(frame);
		}

		count++;
	}
	
	if (p_videoWriter != NULL) {
		delete p_videoWriter;
	}
	if (p_contour != NULL) {
		delete p_contour;
	}
}

