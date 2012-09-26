/**
@file HW01App.cpp
CSE 274 09/05/2012 - HW01

@author Eddie Obissi
Note: All code included on this file is available for public use.
Source code was provided by http://www.dreamincode.net/forums/topic/10157-data-structures-in-c-tutorial/

The code is inclomplete as of yet, because the logic for using the circular linked list is missing.
The list does work, but code needs to be added to make the interface useful to the user. 
Right now it is not.
*/
#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"
#include "cinder/ImageIo.h"
#include "boost/date_time/posix_time/posix_time.hpp"
#include "Resources.h"
#include <cmath>
#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <iostream>

using namespace ci;
using namespace ci::app;
using namespace std;

class HW02_FApp : public AppBasic {
public:
	/* DG I would have to double check with Brinkman but I think the usual convention
	of stating what a method does goes down below where you write in the code for each method
	so that you can state what @param and @return values (if any) that a method may have as well. 
	/**
	This is the default constructor, and must be used. It declares a NULL list
	*/
	HW02_FApp();
	/**
	This is the useful constructor. It copies input values.  This is helpful if we have a list ready for use.
	*/
	HW02_FApp(HW02_FApp& list);
	/**
	This is the list destructor. It will delete the entire list at once.
	*/
	~HW02_FApp();

	void setup();
	void mouseDown( MouseEvent event );	
	void update();
	void draw();
	void prepareSettings(Settings* settings);
private:
	/**
	To make a linked list useful, the nodes must first be initialized.
	*/
	struct node	{
		int shape;
		node* link;
	};
	struct node *n;
	// Linked list funtions


	// In retrospect this would be more useful as array of ints or 
	// seperate object type entirely. It adds an int to the end of the linked list.
	// Right now all it does is chooses a type of shape to draw based on input.
	void addShape(int in);
	
	// This displays the contents of the list, but is not so useful in a cinder application
	void display();
	
	// This gets the size of the linked list
	int count();
	
	// This deletes a shape from the end of the list
	void deleteShape();
	
	// This adds a shape to the front of the linked list based on where the front node is.
	void addShapeFront(int in);

	//Visual and animation functions/values

	Surface* mySurface_; //Our surface

	// For animation
	int frame_number_;
	boost::posix_time::ptime app_start_time_;
		
	// Dimensions of window
	static const int appWidth = 800;
	static const int appHeight = 600;
	static const int surfaceSize = 1024;
	/**
	Draws a rectangle with a border between (x1,y1) and (x2,y2) the logic assumes that the 
	rectangle is declared within the bounds of the window
	*/
	void rectangle(uint8_t* pixels, int x1, int y1, int x2, int y2, Color8u fillColor, Color8u borderColor); 
	
	/**
	Draws a circle with center at (centerX, centerY).  It takes in a radius and a color to further 
	control it's appearance.
	*/
	void circle(uint8_t* surface, int centerX, int centerY, int radius, Color8u fillColor);
	
};

// Initializing our settings.
void HW02_FApp::prepareSettings(Settings* settings)	{
	(*settings).setWindowSize(appWidth, appHeight);
	(*settings).setResizable(false);
}

HW02_FApp::HW02_FApp()	{
	n = NULL;
}

HW02_FApp::HW02_FApp(HW02_FApp& list)	{
	node *r;
	n = NULL;
	r = list.n;
	if(r == NULL)	{
		return;
	}
	for(int i = 1; i < list.count(); i++)	{
		addShape(r->shape);
		r=r->link;
	}
}

HW02_FApp::~HW02_FApp()	{
	node *a, *b;
	a = n; b = n;
	if(n == NULL)
		return;
	while(a->link != b)	{
		n = a; a = a->link; delete n;
	}
	n = a; delete n;
}
/* DG
@param in A shape to be added to the linked list.
*/
 void HW02_FApp::addShape(int in)	{
	if (n == NULL)	{
		node *a; a = new node; 
		a->shape = in; a->link = a; n = a; 
		return;
	}
	node *a; a = n;
	while(a->link != n)
		a = a->link;
	node *b; b = new node;
	b->shape = in; b->link = n; a->link = b;
}

void HW02_FApp::display()	{
	if(n == NULL)	{
		cout << "List is empty\n"; return;
	}
	node *a; a = n;
	for(int i = 1; i <= this->count(); i++)	{
		cout << a->shape << endl; a = a->link;
	}
}

int HW02_FApp::count()	{
	node *a; a = n;
	int c = 0;
	if (n == NULL)	
		return 0; 
	else	c++;
	while(a->link != n)	{
		c++; a = a->link;
	}
	return c;
}

void HW02_FApp::deleteShape()	{
	//DG Instead of using *a, I would use more specific names
	if(n == NULL)	return;
	if(n->link == n) n = NULL;
	else	{
		node *a; a = n;
	while (a->link != n)	
		a = a->link;
	a->link = n->link;
	a = n;
	n = (a->link == NULL ? NULL : n->link);
	delete a;
	}
}
/* DG
@param in A shape to be brought to the front of the linked list.
*/
void HW02_FApp::addShapeFront(int in)	{
	// DG Instead of using *a and *b, I would use more specific names
	node *a, *b; a = n;
	while(a->link != n)
		a->link;
	b = new node; b->shape = in;
	b->link = a; a->link = b; n = b;
}
/* DG
@param pixels the pixels of the image
@param x1 Starting x position of the rectangle
@param y1 Starting y position of the rectangle
@param x2 Ending x position of the rectangle
@param y2 Ending y position of the rectangle
@param fillColor the color of the rectangle
@param borderColor the color of the border of the rectangle
*/
void HW02_FApp::rectangle(uint8_t* pixels, int x1, int y1, int x2, int y2, Color8u fillColor, Color8u borderColor)	{
	
	for (int y = y1; y < y2; y++)	{
		for (int x = x1; x < x2; x++)	{
			// DG Since you use 3*(y*surfaceSize + x) six times throughout
			// this method it would be at easier to just set that whole 
			// formula to an int and plug in the int offset. 
			int offset = 3*(y*surfaceSize + x);
			//Border
			if (std::abs(y - y1) < 4 || std::abs(x - x1) < 4|| std::abs(y - y2) < 4 || std::abs(x - x2) < 4)	{ 
				pixels[offset] = borderColor.r;
				pixels[offset+1] = borderColor.g;
				pixels[offset+2] = borderColor.b;
			}
			//Fill
			else	{
				pixels[offset] = fillColor.r;
				pixels[offset+1] = fillColor.g;
				pixels[offset+2] = fillColor.b;
			}
		}
	}
}
/* DG
@param surface the surface 
@param centerX the x coordinate for the center of the circle
@param centerY the y coordinate for the center of the circle
@param radius the radius of the cirle
@param fillColor the color of the circle
*/
void HW02_FApp::circle(uint8_t* surface, int centerX, int centerY, int radius, Color8u fillColor)	{
	// Square around circle
	for (int j = centerY - radius; j < (centerY + radius); j++)	{
		for (int i = centerX - radius; i < (centerX + radius); i++)	{
			// If inside circle radius, change pixel value
			int distX = std::abs(i - centerX);
			int distY = std::abs(j - centerY);
			if (std::sqrt((double)(distX*distX + distY*distY)) < radius)	{
				//DG Use offset for same reason as explained above.
				int offset = 3*(j*surfaceSize + i);
				surface[offset] = fillColor.r; // Didn't need the + 0
				surface[offset + 1] = fillColor.g;
				surface[offset + 2] = fillColor.b;
			}
		}
	}
}
void HW02_FApp::setup()
{
	frame_number_ = 0;
	mySurface_ = new Surface(surfaceSize, surfaceSize, false);

}

void HW02_FApp::mouseDown( MouseEvent event )
{
	// This is where I'd like to get my inputs based on mouse location
	// However, more information should be included at each node. 
	// An array would be useful
}

void HW02_FApp::update()
{
	uint8_t* dataArray = (*mySurface_).getData();
	// Just showing that we can add values
	// Right now it's tough to show that we can remove them.
	// This is vital for an effective project.

	HW02_FApp a;
	int b = 1 + 0.5*std::sin(2*3.14*.02*frame_number_);// = {0, 100, 200, 100, 150, 200, 0};
	a.addShape(b);
	a.display();
	if (b == 0) {
		circle(dataArray, 150, 150, 150, Color8u(150, 50, 0));
		// DG Testing to see if the following circle will be on top of the previous one.
		(circle(dataArray, 300, 300, 200, Color8u(100, 100, 100)); 
	}
	else
		rectangle(dataArray, 150, 150, 265, 375, Color8u(0, 150, 50), Color8u(250, 150, 50));
	frame_number_++;
}

void HW02_FApp::draw()
{
	// clear out the window with black
	gl::draw(*mySurface_);
}

CINDER_APP_BASIC( HW02_FApp, RendererGl )

