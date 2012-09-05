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
	 /*
	  int wavyX;
	  int wavyY;
	  int wavySign;
	  int waveAngle;
	  */
	  int lineX, lineY, line2X, line2Y, lineSign;

	  struct diamonds_info{
		  int x;
		  int y;
		  int r;
	  };
	  deque<diamonds_info> diamonds_list_;

	  static const int kAppWidth=800;
	  static const int kAppHeight=600;
	  static const int kTextureSize=1024;
	  

  void buildRectangle(uint8_t* pixels, int x1, int x2, int y1, int y2, Color8u fill);

  void drawLine(uint8_t* pixels, int x1, int y1, int x2, int y2, Color8u fill);

  void drawPoint(uint8_t* pixels, int x, int y, Color8u fill);
  
  void drawTriangle(uint8_t* pixels, int x1, int y1,int  x2,int y2, int x3, int y3, Color8u fill);
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


}

void HW01PictureApp::mouseDown( MouseEvent event )
{
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

	brightness += 0.005f;
	if( brightness > 1.0f){
	   brightness = 0.5f;
	}
	
	Color8u c = Color8u(100*brightness,0,0);
	Color8u c2 = Color8u(0,150,150);
	Color8u c3 = Color8u(0, 150, 0);
	Color8u c4 = Color8u(0, 254*brightness, 0);
	Color8u c5 = Color8u(200,200,0);

	
	// Clears the frame every update iteration
	buildRectangle(dataArray, 0, 800, 0, 600, c);

	drawLine(dataArray, 400, 0, 400, 600, Color8u(0,0,0));
	drawLine(dataArray, 0, 300, 800, 300, Color8u(0,0,0));
	

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
	// clear out the window with black
	
	
	//gl::clear( Color( brightness, 0.0, 0.0 ) );
	gl::draw(*mySurface_);
		 

	
}

CINDER_APP_BASIC( HW01PictureApp, RendererGl )
