#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/features2d.hpp>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <map>
#include <vector>
#include <string>
#include <math.h>

#include "GripPipeline.h"

namespace rbl {
	
class Contour {
	private:
		grip::GripPipeline p_pipeline;
	
		std::vector<std::vector<cv::Point> > inputContours, outputContours;
		std::vector<cv::Rect> bounding_rectangles;
		cv::Point Top_left_corner;
		
		std::vector<double> rectangle_height, rectangle_width, centre_x, centre_y;

	public:
		Contour();
		~Contour();
		
		void Process(cv::Mat& source0);
		
		unsigned int GetNumberOfContours();
		std::vector<std::vector<cv::Point> > GetContours();
		std::vector<cv::Rect> GetBoundingRectangles();
		std::vector<double> GetX();
		std::vector<double> GetY();
		std::vector<double> GetHeight();
		std::vector<double> GetWidth();
};

}
