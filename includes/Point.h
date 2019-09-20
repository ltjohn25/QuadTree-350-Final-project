#include <iostream>
#include "Includes.h"
#include "Constants.h"
#include "compgeom.h"
#include "Surface.h"
#include "Event.h"

#ifndef POINT_H
#define POINT_H

class Point
{
public:
	Point(int x, int y, int r, Surface & surface)
	:x_(x), y_(y), radius_(r), surface_(surface)
	{
		color_state = 0;
		color_ = RED;
		int speed = rand() % 5;
		if(speed == 0) 
		{
			speed = (rand() % 2 == 0 ? 1 : -1);
		}
		dx_ = (rand() % 2 == 0 ? speed : speed * -1);
		dy_ = (rand() % 2 == 0 ? speed : speed * -1);

		
	}

	int get_x();
	int get_y();
	int get_radius();
	void draw();
	void move();
	void change_color();
	void change_direction();
	


private:
	int x_, y_, radius_;
	Color color_;
	Surface & surface_;
	int dx_, dy_;
	int color_state;
};

#endif

