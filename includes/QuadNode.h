// File: QuadNode.h
// Author: John Epstein

#ifndef QUADNODE_H
#define QUADNODE_H

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



template < typename T >
class QuadNode
{
public:
	QuadNode(const T & key, Rect & rect, QuadNode * parent=NULL)
	: key_(key), parent_(parent)
	{
		square_.x = rect.x;
		square_.y = rect.y;
		square_.w = rect.w;
		square_.h = rect.h;
	
		threshhold_ = 5;

	}


	//Deconstructor
	~QuadNode()
	{
            std::cout << "This is the DESTRUCTOR!\n";

	}
	void delete_quadtree(QuadNode * p)
        {
            if(p->is_leaf())
            {
                delete p; return;
            }
            else
            {
                for(int i = 0; i < 4; ++i)
                {
                    std::vector<QuadNode *> kid = p->child(i);
                    QuadNode * q = kid[0];
                    delete_quadtree(q);
                }
                delete p;
            }
        }
	void print_children()
	{
		for(int i = 0; this->child(i) != this->rightmost(); ++i)
		{
			std::cout << *(this->child(i)) << '\n';
			
		}
	}



	bool operator==( QuadNode & n ) const
	{
		bool ret = false;
		if(n.get_key() == this->key_)
		{
			if(this->num_children() == n.num_children())
			{
				int temp = this->child_.size();
				if(temp == 0) //a LEAF
				{
					ret = true;
				}
				else
				{
					for(int i = 0; i < temp; ++i)
					{
						if(this->child(i) == n.child(i))
						{
							ret = true;
						}
						else
						{
							ret = false;
							break;
						}
						
					}

				}
				
			}
			ret = true;
		}
		return ret;
	}


	int size()
	{
		int count = 0;
		find_size(count,this);
		return count;
	}

	void find_size(int & count, QuadNode<T> * p)
	{
		
		++count;
		if(p->is_leaf()) return;
		else
		{
		       
			for(int i = 0; i < p->num_children(); ++i)
			{
				return find_size(count, p->child(i));
			}

		}
	}

	void add_child(QuadNode * n) // new child value
	{
	
		child_.push_back(n);
	}
	

	const QuadNode * const parent() const
	{
		return parent_;	
	}


	T get_key() 
	{
		return key_;
	}



	bool is_root() const
	{
		return (parent_ == NULL);
	}


	bool is_leaf() const
	{
		return (child_[0].size() == 0);
	}

	bool is_nonleaf() const
	{
		return (!is_leaf());

	}

        std::vector<QuadNode *> child(int n)
        {
            return child_[n];
        }
	T get_key(int i)
	{
		return this->key_[i];

	}



	Rect get_rect()
	{
		return square_;
	}


	

	void insert(Point * p, std::vector<Rect> & boxes)
	{

		
            if(child_[0].size() == 0) 
		{
			if( key_.size() + 1 <= threshhold_)
			{
				key_.push_back(p);
			}
			else
			{
				
				//exceeds threshold so create 4 children 
				// and put p pointer into correct child

				std::vector<Point *> left_top; 
				std::vector<Point *> right_top; 
				std::vector<Point *> left_bottom;
				std::vector<Point *> right_bottom;

				for(int i = 0; i < key_.size(); ++i)
				{
					int x = key_[i]->get_x();
					int y = key_[i]->get_y();
					if(x >= square_.w / 2 + square_.x)//right half
					{
						if(y >= square_.h / 2 + square_.y) //bottom half
						{
							Point * q = key_[i];
							right_bottom.push_back(q);
							
						}
						else //  top half
						{
							Point * q = key_[i];
							right_top.push_back(q);

						}

					}
					else //left half
					{
						if(y >= square_.h / 2 + square_.y) //bottom half
						{
							Point * q = key_[i];
							left_bottom.push_back(q);

						}
						else //  top half
						{
							//Q1
							Point * q = key_[i];
							left_top.push_back(q);

						}

					}

				}
		

				// ADD P INTO CORRECT Q-VECTOR!!!!!

				if(p->get_x() >= square_.w / 2 + square_.x)
				{
                                    if(p->get_y() >= square_.h / 2 + square_.y)
                                    {
                                            
						
                                        Point * q = p;
                                        right_bottom.push_back(q);
					
                                    }
                                    else
                                    {
					
                                        Point * q = p;
                                        right_top.push_back(q);
						
                                    }
				}
				else
				{
                                    if(p->get_y() >= square_.h / 2 + square_.y)
                                    {
					
                                        Point * q = p;
                                        left_bottom.push_back(q);
					
                                    }
                                    else



                                            
					{
					
						Point * q = p;
						left_top.push_back(q);
					
					}
				}

			
				//make 4 new Rects for each quad Node
				Rect * r1 = new Rect(square_.x, square_.y, square_.w / 2, square_.h / 2);
				Rect * r2 = new Rect(square_.x + square_.w / 2, square_.y, square_.w / 2, square_.h / 2);
				Rect * r3 = new Rect(square_.x, square_.y + square_.h / 2, square_.w / 2, square_.h / 2);
				Rect * r4 = new Rect(square_.x + square_.w / 2, square_.y + square_.h / 2, square_.w / 2, square_.h / 2);


				
			
				boxes.push_back(*r1);
				boxes.push_back(*r2);
				boxes.push_back(*r3);
				boxes.push_back(*r4);


				//fill each vector with pointers
				//create new children Nodes with- q vector
			
				QuadNode * p1 = new QuadNode(left_top,*r1,this);
				QuadNode * p2 = new QuadNode(right_top,*r2,this);
				QuadNode * p3 = new QuadNode(left_bottom,*r3,this);
				QuadNode * p4 = new QuadNode(right_bottom,*r4,this);

			

				child_[0].push_back(p1);
				child_[1].push_back(p2);
				child_[2].push_back(p3);
				child_[3].push_back(p4);


				

				delete r1;
				delete r2;
				delete r3;
				delete r4;


			
			}
		}
		else
		{
			//children already exist so call insert to correct child
			if(p->get_x() >= square_.w / 2 + square_.x)
			{
				if(p->get_y() >= square_.h / 2 + square_.y)
				{
				
					child_[3][0]->insert(p,boxes);
				}
				else
				{
				
					child_[1][0]->insert(p,boxes);
				}


			}
			else
			{
				if(p->get_y() >= square_.h / 2 + square_.y)
				{
					
					child_[2][0]->insert(p,boxes);
				}
				else
				{	
					child_[0][0]->insert(p,boxes);
				}


			}
		}
		
	}

private:
	T key_;
	QuadNode * parent_;
	std::vector< QuadNode * > child_[4];
	int threshhold_;
	Rect square_;
};
#endif
