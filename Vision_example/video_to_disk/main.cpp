#include <cscore.h>
#include <opencv2/core/core.hpp>
#include <opencv2/videoio/videoio.hpp>
#include <iostream>

using namespace cs;
using namespace cv;

int main() {
	
	int width = 320;
	int height = 240;
	int fps = 15;
	
	UsbCamera camera("usbcam", 0);
	
	//SetVideoMode (VideoMode::PixelFormat pixelFormat, int width, int height, int fps)
	camera.SetVideoMode(VideoMode::PixelFormat::kMJPEG, width, height, fps);
	
	MjpegServer mjpegServer("httpserver", 8081);
	mjpegServer.SetSource(camera);
	CvSink cvsink("cvsink");
	cvsink.SetSource(camera);
	CvSource cvsource("cvsource",
			VideoMode::PixelFormat::kMJPEG, width, height, fps);
	MjpegServer cvMjpegServer("cvhttpserver", 8082);
	cvMjpegServer.SetSource(cvsource);

	VideoWriter videoWriter("output.avi", VideoWriter::fourcc('F', 'M', 'P', '4'), fps, Size(width, height), true);	
	
	if (!videoWriter.isOpened())
	{
		std::cout << "Cannot open output file " << std::endl;

		return 1;
	}

	Mat frame;

	int count = 0;
	while (count < 100) {
		uint64_t time = cvsink.GrabFrame(frame);
		std::cout << "Frame Time: " << time << std::endl;
		videoWriter.write(frame);
		count++;
	}
}
