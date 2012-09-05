/**
*@file HW01Picture.cpp
*CSE 274 - Fall 2012
*A solution for HW01
*
*@author Nicholas Collins
*@date 2012-09-05
*
*@note This project satisfies goals A.1 (rectangle), A.2 (circle), 
* A.3 (line), A.7 (triangle), E.5 (animation), and E.6 (mouse interaction)
*/ 





#pragma once
#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"
#include "cinder/ImageIo.h"
#include "boost/date_time/posix_time/posix_time.hpp"
#include "Resources.h"
#include <cmath>

using namespace ci;
using namespace ci::app;
using namespace std;

class HW01PictureApp : public AppBasic {
  public:
	void setup();
	void mouseDown( MouseEvent event );	
	void update();
	void draw();
	void prepareSettings(Settings* settings);

  private:
	  float brightness;
	  float xDiff, yDiff;
	  float xSign, ySign;
	  int sign, trigXSign, trigYSign, trigXDiff, trigYDiff;

	  Surface* mySurface_;
	  int lineX, lineY, line2X, line2Y, lineSign;
	  uint8_t* blur_pattern_;

	  struct diamonds_info{
		  int x;
		  int y;
		  int r;
	  };
	  deque<diamonds_info> diamonds_list_;

	  static const int kAppWidth=800;
	  static const int kAppHeight=600;
	  static const int kTextureSize=1024;
	  
  /**
   * Fill a solid color rectangle between the given x1,x2,y1,and y2 
   * parameters. 
   *
   *Satisfies the rectangle requirement, A.1
   */
  void buildRectangle(uint8_t* pixels, int x1, int x2, int y1, int y2, Color8u fill);

  /**
   * Draw a line, of a 1 pixel width between the given
   * endpoints (x1, y1) , (x2, y2). Uses the Bresenham's Line
   * algorithm
   * 
   * Fulfills the line requirement, A.3
   */
  void drawLine(uint8_t* pixels, int x1, int y1, int x2, int y2, Color8u fill);

  /**
   * Draw a single point, given an x and y coordinate
   * Only meant to simplify thought process
   */
  void drawPoint(uint8_t* pixels, int x, int y, Color8u fill);
  
  /**
   * Draws an empty triangle with the endpoints
   * (x1,y1), (x2,y2), and (x3,y3)
   * By making three calls to the drawLine method
   *
   * Fulfills the triangle requirement, A.7
   */
  void drawTriangle(uint8_t* pixels, int x1, int y1,int  x2,int y2, int x3, int y3, Color8u fill);

  /**
   * Draws a circle at the (x,y) coordinate, with a radius of
   * r. Uses the axis-aligned bounding box method
   *
   * Fulfills the circle requirement, A.2
   */
  void makeCircle(uint8_t* pixels, int x, int y, int r, Color8u fill);

  /**
   * Blurs an image (pixels) using blur_pattern
   * Not fully implemented
   *
   * Fulfills the blur requirement, B.1
   */
  void blur(uint8_t* pixels, uint8_t* blur_pattern);
};

void HW01PictureApp::prepareSettings(Settings* settings){
	(*settings).setWindowSize(800,600);
	(*settings).setResizable(false);
}

void HW01PictureApp::buildRectangle(uint8_t* pixels, int x1, int x2, int y1, int y2, Color8u fill){

	Color8u c = fill;

	for(int i = x1; i<x2; i++){
		for(int j = y1; j<y2; j++){
	
	
	     pixels[3*(i+j*1024)] = c.r;
		 pixels[3*(i+j*1024)+1] = c.g;
		 pixels[3*(i+j*1024)+2] = c.b;


		}

	}

}

void HW01PictureApp::drawPoint(uint8_t* pixels, int x, int y, Color8u fill){
	
	Color8u c = fill;
	pixels[3*(x+y*1024)] = c.r;
	pixels[3*(x+y*1024)+1] = c.g;
	pixels[3*(x+y*1024)+2] = c.b;


}

void HW01PictureApp::drawLine(uint8_t* pixels,int x1, int y1, int x2, int y2, Color8u fill){

	Color8u c = fill;
	// Following code implements Bresenham's Line Algorithm, inspired by:
	// http://roguebasin.roguelikedevelopment.org/index.php/Bresenham's_Line_Algorithm
	int delta_x(x2 - x1);
    // if x1 == x2, then it does not matter what we set here
    signed char const ix((delta_x > 0) - (delta_x < 0));
    delta_x = std::abs(delta_x) << 1;
 
    int delta_y(y2 - y1);
    // if y1 == y2, then it does not matter what we set here
    signed char const iy((delta_y > 0) - (delta_y < 0));
    delta_y = std::abs(delta_y) << 1;
 
    drawPoint(pixels, x1, y1, c);
 
    if (delta_x >= delta_y)
    {
        // error may go below zero
        int error(delta_y - (delta_x >> 1));
 
        while (x1 != x2)
        {
            if ((error >= 0) && (error || (ix > 0)))
            {
                error -= delta_x;
                y1 += iy;
            }
            // else do nothing
 
            error += delta_y;
            x1 += ix;
 
            drawPoint(pixels, x1, y1, c);
        }
    }
    else
    {
        // error may go below zero
        int error(delta_x - (delta_y >> 1));
 
        while (y1 != y2)
        {
            if ((error >= 0) && (error || (iy > 0)))
            {
                error -= delta_y;
                x1 += ix;
            }
            // else do nothing
 
            error += delta_x;
            y1 += iy;
 
            drawPoint(pixels,x1, y1,c);
        }
    }
}

void HW01PictureApp::drawTriangle(uint8_t* pixels, int x1, int y1, int x2, int y2, int x3, int y3, Color8u fill){

	Color8u c = fill;

	drawLine(pixels, x1, y1, x2, y2, c);
	drawLine(pixels, x2, y2, x3, y3, c);
	drawLine(pixels, x3, y3, x1, y1, c);

}

void HW01PictureApp::makeCircle(uint8_t* pixels, int x, int y, int r, Color8u fill){

	if(r<=0) return;
	Color8u c =fill;
	
	// Axis aligned bounding box method
	for(int i = x-r;i<(x+r);i++){
		for( int j = y-r; j<(y+r);j++){
			// Check bounds
			if(j < 0 || i < 0 || i >= 800 || j >= 600) continue;
			int  dist = (int)sqrt((double)((i-x)*(i-x))+((j-y)*(j-y)));
			if(dist<=r) drawPoint(pixels, i,j, c);
			
				
		}
	}

}

void HW01PictureApp::blur(uint8_t* pixels, uint8_t* blur_pattern){

	// This method borrows ideas from both Bo's solution
	// and http://cboard.cprogramming.com/cplusplus-programming/62752-blur-sharpen-bitmap.html
	static uint8_t image_copy[3*1024*1024];

	uint8_t kernel[9] = {1,2,1,2,4,2,1,2,1};
	int k, total_red, total_green, total_blue;

	for(int i = 1; i<(600-1); i++){
		for(int j = 1; j<(800-1);j++){

			int offset = 3*(i+j*800);
			total_red = 0;
			total_green = 0;
			total_blue = 0;

			for( int k = 0; k < 3; k++){
				for( int l =0; l< 3; k++){

					offset = 3*(j+l + (i+k)*1024);
					k= kernel[l+1+(k+1)*3];

					total_red += (image_copy[offset] >> k);
					total_green += (image_copy[offset+1] >>k);
					total_blue += (image_copy[offset+2] >>k);
				}
			}
			offset = 3*(j+i*1024);
			pixels[offset] = total_red;
			pixels[offset] =total_green;
			pixels[offset] = total_blue;
		}
	}



}



void HW01PictureApp::setup()
{
	brightness = 0.5f;
	mySurface_ = new Surface(kTextureSize,kTextureSize,false);
	xSign = 1.0;
	ySign = 1.0;
	xDiff = 0.0;
	yDiff = 0.0;
	trigXDiff = 0;
	trigYDiff =0;
	sign =1;
	trigXSign = 1;
	trigYSign =1;
	lineX=0;
	lineY=0;
	lineSign =1;
	line2X = 0;
	line2Y = 600;

	/*
	blur_pattern_ = new uint8_t[800*600*3];
	for(int i=0;i<600;i++){
		for(int j=0;j<800;j++){
			int offset = 3*(j + i*800);
			blur_pattern_[offset] = dataArray[offset];
		}
	}
	*/



}

// Mouse interaction fulfills goal E.7
void HW01PictureApp::mouseDown( MouseEvent event ){
	
	
	diamonds_info t;
	t.x = event.getX();
	t.y = event.getY();
	t.r = 50;
	sign = sign*-1;
	diamonds_list_.push_back(t);

}

void HW01PictureApp::update()
{
	uint8_t* dataArray = (*mySurface_).getData();

	// Regulates the color of the background
	brightness += 0.005f;
	if( brightness > 1.0f){
	   brightness = 0.5f;
	}
	
	Color8u c = Color8u(100*brightness,0,0);
	Color8u c2 = Color8u(0,150,150);
	Color8u c3 = Color8u(0, 150, 0);
	Color8u c4 = Color8u(0, 254*brightness, 0);
	Color8u c5 = Color8u(200,200,0);
	Color8u c6 = Color8u(0,0,255);

	
	// Clears the frame every update iteration
	buildRectangle(dataArray, 0, 800, 0, 600, c);

	// Draw the four circles
	makeCircle(dataArray, 200,150, 100, c6);
	makeCircle(dataArray, 200,450,100, Color8u(255,255,0));
	makeCircle(dataArray, 600,450, 100, Color8u(255,0,255));
	makeCircle(dataArray, 600,150,100, Color8u(0,255,255));

	// Draw the lines that make the "frame"
	drawLine(dataArray, 400, 0, 400, 600, Color8u(0,0,0));
	drawLine(dataArray, 0, 300, 800, 300, Color8u(0,0,0));
	
	//blur(dataArray, blur_pattern_);

	// The following conditionals are responsible for the 
	// animations of the rectangle and triangle
	// Fulfills goal E.6, for animation
	if((400+xDiff)>= 800.0 || (200+xDiff) <= 0.0) xSign = xSign*(-1.0);
	xDiff = 2.0*xSign+xDiff;
	if((300+yDiff)>= 600.0 || (100+yDiff)<= 0.0) ySign = ySign*(-1.0);
	yDiff = 2.0*ySign+yDiff;
	
	if((trigXDiff + 266) <= 0 || (533+trigXDiff)>= 800 )
		trigXSign = trigXSign*(-1);
	trigXDiff = 3*trigXSign+trigXDiff;

	if((trigYDiff +200) <= 3|| (400+trigYDiff)>=600)
		trigYSign = trigYSign*(-1);
	trigYDiff = 3*trigYSign+trigYDiff;
	
	buildRectangle(dataArray, 200+xDiff, 400+xDiff, 100+yDiff, 300+yDiff, c2);

	drawTriangle(dataArray, 266+trigXDiff, 400+trigYDiff, 400+trigXDiff, 200+trigYDiff, 533+trigXDiff, 400+trigYDiff, c3);
	 
	// Following few lines of code are responsible for
	// the sliding line segments
	drawLine(dataArray, lineX, lineY, lineX+100, lineY+75, c4);
	drawLine(dataArray, line2X, line2Y, line2X+100, line2Y-75, c4);
	lineX +=4*lineSign;
	lineY +=3*lineSign;
	line2X += 4*lineSign;
	line2Y += -3*lineSign;

	if(lineX == 696) lineSign = lineSign*-1;
	if(lineX < 0){
		lineSign = lineSign*-1;
		lineX+=4;
		lineY+=3;
		line2X+=4;
		line2Y-=4;
	}

	
	
	
	// The rest of the update method is used for mouse
	// interaction and event handling
	while(diamonds_list_.size() > 0 && diamonds_list_[0].r<=0)
		diamonds_list_.pop_front();

	
	for(int i=0; i<diamonds_list_.size();i++){

		
		diamonds_info t = diamonds_list_[i];
		drawTriangle(dataArray, t.x, t.y, t.x, t.y-t.r, t.x+t.r, t.y,c5 );
		drawTriangle(dataArray, t.x, t.y, t.x, t.y+t.r, t.x+t.r, t.y,c5 );
		drawTriangle(dataArray, t.x, t.y, t.x, t.y-t.r, t.x-t.r, t.y,c5 );
		drawTriangle(dataArray, t.x, t.y, t.x, t.y+t.r, t.x-t.r, t.y,c5 );
		if(diamonds_list_[i].r > 75)
			sign = sign*-1;

		diamonds_list_[i].r +=3*sign;


	}
	
	
	
	
}

void HW01PictureApp::draw()
{
	
	gl::draw(*mySurface_);
		 

	
}

CINDER_APP_BASIC( HW01PictureApp, RendererGl )
