#ifndef TEMPMEDIANFILTER
#define TEMPMEDIANFILTER
#include "opencv2\objdetect\objdetect.hpp"
#include <opencv\cv.h>
#include <opencv\highgui.h>
#include <iostream>
#include <fstream>
#include <stdint.h>

using namespace cv;
using namespace std;


class TempMedianFilter
{
private:
	int nPrevFrames,frameNum,width,height,nFramesIdx;
	uchar*  curFrame;
	vector<vector<int>> median,lowerBound,upperBound;
	vector<vector<vector<int> > > updatedHist;
	vector<uchar*> nFrames;
	void updateBounds(int x,int y);
	int repeatitionCheck(int x,int y);
public:
	TempMedianFilter(int _nPrevFrames,int _width,int _height);
	void operator()(InputArray& _src, OutputArray& _dst);
};

#endif