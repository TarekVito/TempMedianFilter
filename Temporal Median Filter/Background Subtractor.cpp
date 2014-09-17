#include<iostream>
#include "TempMedianFilter.h"

const int nPrevFrames = 10;

int main()
{
	VideoCapture cap(1);  
	Mat frame;
	if(!cap.isOpened())   
		return -1;
	cap>>frame;
	TempMedianFilter bgTMF(nPrevFrames,frame.cols,frame.rows); 
	Mat background,foreground1,foreground2;
	while(true)
	{
		cap>>frame;
		cvtColor(frame,frame,COLOR_RGB2GRAY);
		clock_t ct = clock();
		bgTMF(frame,background);
		absdiff(background,frame,foreground1);
		threshold(foreground1,foreground1,50,255,CV_THRESH_BINARY);

		imshow("Background1",foreground1);
		//imshow("Background2",foreground2);
		waitKey(10);
	}
}