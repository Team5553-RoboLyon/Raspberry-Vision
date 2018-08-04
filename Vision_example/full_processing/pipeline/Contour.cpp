#include "Contour.h"

namespace rbl {

Contour::Contour()
{
	
}

Contour::~Contour()
{
	
}

void Contour::Process(cv::Mat& source0)
{
	p_pipeline.Process(source0);
	
	inputContours = *p_pipeline.GetFilterContoursOutput();
	
	bounding_rectangle.clear();
	rectangle_height.clear();
	rectangle_width.clear();
	centre_x.clear();
	centre_y.clear();
	
	for (std::vector<cv::Point> contour: inputContours)
	{
		bounding_rectangle.push_back(boundingRect(contour));
		
		rectangle_height.push_back(bounding_rectangle.back().height);
		rectangle_width.push_back(bounding_rectangle.back().width);
		Top_left_corner = bounding_rectangle.back().tl();
		centre_x.push_back(Top_left_corner.x + (bounding_rectangle.back().width/2));
		centre_y.push_back(Top_left_corner.y + (bounding_rectangle.back().height/2));
	}
}

unsigned int Contour::GetNumberOfContours()
{
	return inputContours.size();
}

std::vector<cv::Rect> Contour::GetBoundingRectangle()
{
	return bounding_rectangle;
}

std::vector<std::vector<cv::Point> > Contour::GetContours()
{
	return inputContours;
}

std::vector<double> Contour::GetX()
{
	return centre_x;
}

std::vector<double> Contour::GetY()
{
	return centre_y;
}

std::vector<double> Contour::GetHeight()
{
	return rectangle_height;
}

std::vector<double> Contour::GetWidth()
{
	return rectangle_width;
}
};
