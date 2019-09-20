/****************************************************************************
 Yihsiang Liow
 Copyright
 ****************************************************************************/
#include <iostream>
#include <cmath>
#include <vector>
#include <string>
#include <fstream>
#include <ctime>
#include <cmath>
#include <cstdlib>
#include <sstream>

#include "Includes.h"
#include "Constants.h"
#include "compgeom.h"
#include "Surface.h"
#include "Event.h"
#include "Point.h"
#include "QuadNode.h"


template <typename T>
std::ostream & operator<<(std::ostream & cout, const std::vector<T> v)
{
    cout << '[';
    for(int i = 0; i < v.size(); ++i)
    {
        cout << *(v[i]);
        if(i != v.size() - 1)
        {
            cout << ',';
        }
    }
    cout << ']';
    return cout;
}

std::ostream & operator<<(std::ostream & cout, Point & d)
{
	
	cout << '(';
	
	cout << d.get_x() << ',' << d.get_y();
	cout << ')';
	return cout;
}


std::ostream & operator<<(std::ostream & cout, Rect & r)
{
	cout << "Rect: [";
	cout << r.x << ',' << r.y << ',';
	cout << r.w << ',' << r.h << "]\n";
	return cout;
}



bool is_collision(Point & a, Point & b)
{
	int max_x = max(a.get_x(), b.get_x());
	int max_y = max(a.get_y(), b.get_y());
	int min_x = (max_x == a.get_x() ? b.get_x() : a.get_x());
	int min_y = (max_y == a.get_y() ? b.get_y() : a.get_y());
	int height = max_y - min_y;
	int length = max_x - min_x;
	double distance = sqrt(height * height + length * length);
	if(distance <= 2 * a.get_radius())
	{
		return true;
	}
	else return false;
	
}




void brute_force(std::vector<Point * > & v)
{
	for(int i =  0; i < v.size(); ++i)
	{
		for(int j = i + 1; j < v.size(); ++j)
		{

			if(is_collision(*v[i], *v[j]))
			{
				//std::cout << "HERE\n";
				v[i]->change_color();
				v[j]->change_color();
				v[i]->change_direction();
				v[j]->change_direction();
	
			}


		}
	}
	
}

void tree_collisions(QuadNode<std::vector<Point *>> * n)
{
    if(n->is_leaf())
    {
        std::vector<Point *> key = n->get_key();

        for(int i = 0; i < key.size(); ++i)
        {
            for(int j = i + 1; j < key.size(); ++j)
            {
                if(is_collision(*key[i], *key[j]))
                {
                    key[i]->change_color();
                    key[j]->change_color();
                    key[i]->change_direction();
                    key[j]->change_direction();
                }
            }
        }
        return;
    }
    else
    {
        for(int i = 0; i < 4; ++i)
        {
            std::vector<QuadNode<std::vector<Point *> > *> kids = n->child(i);	
            QuadNode<std::vector<Point *>> * temp = kids[0];

            tree_collisions(temp);

        }
    }
}
void draw_boxes(std::vector< Rect > & v, Surface & surface)
{
	for(int i = 0; i < v.size(); ++i)
	{
		Rect temp = v[i];
		int x0 = temp.x;
		int y0 = temp.y;

		int x1 = temp.x + temp.w - 1;
		int y1 = temp.y;

		int x2 = x1;
		int y2 = temp.y + temp.h - 1;

		int x3 = x0;
		int y3 = y2;
		//std::cout << "w: " << x << '\n'; 
		surface.put_line(x0,y0,x1,y1, WHITE);
		surface.put_line(x1,y1,x2,y2, WHITE);
		surface.put_line(x2,y2,x3,y3, WHITE);
		surface.put_line(x0,y0,x3,y3, WHITE);
	}
	v.clear();

}

void build_quadtree(QuadNode<std::vector<Point *>> & n,
 						std::vector<Point *> c, std::vector<Rect> & r)
{
	for(int i = 0; i < c.size(); ++i)
	{
		n.insert(c[i], r);
	}

}

void game_function()
{

	
	Surface surface(W,H);
	Rect window(0,0,W,H);
	Event event;
	std::vector<Point * > circles;


	int radius = 4;
	
	//init of points
	int num_of_points = 300;
	for(int i = 0; i < num_of_points; ++i)
	{
		int x = rand() % W;
		int y = rand() % H;
		if(y < radius)
		{
			y += radius;
		}
		if(x < radius)
		{
			x += radius;
		}
		circles.push_back(new Point(x,y,radius,surface));
	}


	 std::vector<Point *> v1 = {};

	std::vector< Rect > boxes_;
	boxes_.push_back(window);	




	

	while(1)
	{
		if(event.poll() && event.type() == QUIT) break;
                KeyPressed keypressed = get_keypressed();

		
	
		std::vector<Point *> e = {};
		QuadNode<std::vector<Point * >> * tree = new QuadNode<std::vector<Point *>>(e,window,NULL);



		//Moving
		//*************************************
		for(int i = 0; i < circles.size(); ++i)
		{
			circles[i]->move();
		}
		//**************************************




	
		// COLLISION DETECTION
		// ********************************
		//brute_force(circles);

		//  OR
                	if(keypressed[SPACE])
		{
			brute_force(circles);
		}
		//brute_force(circles);
		else
		{
			build_quadtree(*tree,circles, boxes_);


			tree_collisions(tree);
		}

		

		
		surface.lock();
		surface.fill(BLACK);

		//draw points
		for(int i = 0; i < circles.size(); ++i)
		{	
			circles[i]->draw();
		}

		draw_boxes(boxes_,surface);

		surface.unlock();
		surface.flip();
		
		delay(10);

	}

	return;
	
}




int main(int argc, char* argv[])
{

    srand(2);

    game_function();
	return 0;
}
