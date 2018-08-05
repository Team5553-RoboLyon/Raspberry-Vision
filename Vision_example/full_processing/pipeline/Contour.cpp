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
	
	//Suprimer les infos précédantes
	outputContours.clear();
	bounding_rectangles.clear();
	rectangle_height.clear();
	rectangle_width.clear();
	centre_x.clear();
	centre_y.clear();
	
	//Tester pour chacun des contours si il est dans un autre contour
	for(unsigned int contour_a_tester_1 = 0; contour_a_tester_1 < inputContours.size(); contour_a_tester_1++)
	{
		bool contour_accepte = true;
		cv::Rect rectangle_a_tester_1 = boundingRect(inputContours[contour_a_tester_1]);
		
		for(unsigned int contour_a_tester_2 = 0; contour_a_tester_2 < inputContours.size(); contour_a_tester_2++)
		{
			//On ne va pas comparer un contour à lui même
			if(contour_a_tester_1 != contour_a_tester_2)
			{
				cv::Rect rectangle_a_tester_2 = boundingRect(inputContours[contour_a_tester_2]);
				
				//On teste si le premier contour est dans le second
				if( rectangle_a_tester_1.tl().x > rectangle_a_tester_2.tl().x && 
					rectangle_a_tester_1.tl().y > rectangle_a_tester_2.tl().y && 
					rectangle_a_tester_1.height < rectangle_a_tester_2.height && 
					rectangle_a_tester_1.width < rectangle_a_tester_2.width)
				{
					contour_accepte = false;
				}
			}
		}
		
		//Si le contour est accepté on le rajoute à la liste outputContours et on rajoute "son rectangle" à la liste bounding_rectangles
		if(contour_accepte)
		{
			outputContours.push_back(inputContours[contour_a_tester_1]);
			bounding_rectangles.push_back(rectangle_a_tester_1);
		}
	}
	
	//Recuperer les infos pour chaque rectangle
	for (cv::Rect rectangle: bounding_rectangles)
	{
		rectangle_height.push_back(rectangle.height);
		rectangle_width.push_back(rectangle.width);
		Top_left_corner = rectangle.tl();
		centre_x.push_back(Top_left_corner.x + (rectangle.width/2));
		centre_y.push_back(Top_left_corner.y + (rectangle.height/2));
	}
}

unsigned int Contour::GetNumberOfContours()
{
	return bounding_rectangles.size();
}

std::vector<cv::Rect> Contour::GetBoundingRectangles()
{
	return bounding_rectangles;
}

std::vector<std::vector<cv::Point> > Contour::GetContours()
{
	return outputContours;
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
