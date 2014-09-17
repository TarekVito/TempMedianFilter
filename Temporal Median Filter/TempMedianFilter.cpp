#include "TempMedianFilter.h"

TempMedianFilter::TempMedianFilter(int _nPrevFrames,int _width,int _height)
{
	nPrevFrames  = _nPrevFrames;
	frameNum=1;
	nFramesIdx = 1;
	width = _width;
	height = _height;
	updatedHist = vector<vector<vector<int> > >(height);
	lowerBound = upperBound = median = vector<vector<int> >(height);
	
	nFrames = vector<uchar* >(_nPrevFrames);
	for(int w=0;w<nPrevFrames;++w)
		nFrames[w] = new uchar[width*height];
	
	memset(nFrames[0],0,sizeof(nFrames[0]));
	for(int i=0;i<height;++i)
	{
		median[i].resize(width);
		upperBound[i] = lowerBound[i] = median[i];
		updatedHist[i].resize(width);
		for(int j=0;j<width;++j)
			updatedHist[i][j].resize(256),updatedHist[i][j][1]=1,upperBound[i][j]=1;
	}
}
void TempMedianFilter::updateBounds(int x,int y)
{
	int direction = repeatitionCheck(x,y);
	if(!direction)
		return;
	int sum =0;
	for(int i=0;i<256;i++)
	{
		sum+=updatedHist[x][y][i];
		if(sum>= (frameNum/2))
		{
			lowerBound[x][y]= sum-updatedHist[x][y][i];
			upperBound[x][y]= sum;
			median[x][y] = i;
			return;
		}
	}
}
void TempMedianFilter::operator()(InputArray& _src, OutputArray& _dst)
{
	curFrame = _src.getMat().data;
	frameNum +=  frameNum < nPrevFrames?1:0;
	_dst.create(_src.getMat().size(), _src.getMat().type());
	uchar* dstData = _dst.getMat().data;
	for(int i=0;i<height;++i)
	{
		for(int j=0;j<width;++j)
		{
			updatedHist[i][j][curFrame[i*width + j]]++;
			if(frameNum == nPrevFrames) 
				updatedHist[i][j][nFrames[(nFramesIdx+1)%nPrevFrames][i*width + j]]--;
			
			updateBounds(i,j);

			nFrames[(nFramesIdx)%nPrevFrames][i*width + j] = curFrame[i*width + j];
			dstData[i*width + j] = median[i][j];
		}
	}
	nFramesIdx = (nFramesIdx+1)%nPrevFrames;
	return;
}
int TempMedianFilter::repeatitionCheck(int x,int y)
{
	int deltaLB[9] = {0,-1,-1,1,0,0,1,0,0};
	int deltaUB[9] = {0,0,-1,0,0,-1,1,1,0};
	int a,b;

	if(frameNum<nPrevFrames)
		a=2;
	else if(nFrames[(nFramesIdx+1)%nPrevFrames][x*width + y]<median[x][y])
		a=0;
	else if(nFrames[(nFramesIdx+1)%nPrevFrames][x*width + y]==median[x][y])
		a=1;
	else
		a=2;
	
	if(curFrame[x*width + y]<median[x][y])
		b=0;
	else if(curFrame[x*width + y]==median[x][y])
		b=1;
	else
		b=2;
	lowerBound[x][y] += deltaLB[3*a + b];
	upperBound[x][y] += deltaUB[3*a + b];
	return (frameNum/2)<lowerBound[x][y]?-1:(((frameNum/2) > upperBound[x][y])?1:0);
}

