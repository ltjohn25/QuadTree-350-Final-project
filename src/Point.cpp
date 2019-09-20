#include <iostream>
#include "Includes.h"
#include "Constants.h"
#include "compgeom.h"
#include "Surface.h"
#include "Event.h"
#include "Point.h"

void Point::draw()
{
	surface_.put_circle(x_,y_,radius_, color_.r, color_.g, color_.b);
}


int Point::get_x() 
{
	return x_;

}

int Point::get_y() 
{
	return y_;

}

int Point::get_radius()
{
	return radius_;
}

void Point::move()
{
	if((x_ + radius_) + dx_ >= W || (x_ - radius_) + dx_ <= 0)
	{
		dx_ *= -1;
	}
	x_ += dx_;

	if((y_ + radius_) + dy_ >= H || (y_ - radius_) + dy_ <= 0)
	{
		dy_ *= -1;
	}
	y_ += dy_;

}


void Point::change_color()
{
	// RED   - 0
	// GREEN - 1

	color_ = (color_state == 0 ? PURPLE : RED);
	color_state = (color_state == 0 ? 1 : 0);

}


void Point::change_direction()
{
	dx_ *= -1;
	dy_ *= -1;
}
