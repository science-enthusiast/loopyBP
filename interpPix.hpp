#ifndef INTERPPIX_HPP
#define INTERPPIX_HPP

#include <math.h>
#include <vector>
#include <stdlib.h>
//#include <cv.h>

#include "highgui\highgui.hpp"

typedef std::vector< std::vector<double> > VEC_VEC;

double interpPix(double rPos, double cPos, const cv::Mat& ipImg)
{
   if (rPos < 0)
   {
	   rPos = 0;
   }
   else if (cPos < 0)
   {
	   cPos = 0;
   }

   int rowTopL = int(std::floor(rPos));
   int colTopL = int(std::floor(cPos));

   int rowBotL = 0, colBotL = 0, rowBotR = 0, colBotR = 0, rowTopR = 0, colTopR = 0;

   if (rPos >= (ipImg.rows-1))
   {
      rowBotL = rowTopL;
      rowBotR = rowTopL;
   }
   else
   {
      rowBotL = rowTopL + 1;
      rowBotR = rowTopL + 1;
   }
  
   if (cPos >= (ipImg.cols-1))
   {
      colTopR = colTopL;
      colBotR = colTopL;
   }
   else
   {
      colTopR = colTopL + 1;
      colBotR = colTopL + 1; 
   }

#if 1 //bilinear interpolation according to wikipedia
   double x = cPos-colBotL;
   double y = rowBotL - rPos;

   std::vector<double> lVec(2);
   std::vector<double> rVec(2);
   std::vector< std::vector<double> > pixMat(2);

   lVec[0] = 1 - x;
   lVec[1] = x;
   rVec[0] = 1 - y;
   rVec[1] = y;

   std::vector<double> pixVec(2);

   pixVec[0] = static_cast<double>(ipImg.at<uchar>(rowBotL,colBotL));
   pixVec[1] = static_cast<double>(ipImg.at<uchar>(rowBotR,colBotR));
   pixMat[0] = pixVec;

   pixVec[0] = static_cast<double>(ipImg.at<uchar>(rowTopL,colTopL));
   pixVec[1] = static_cast<double>(ipImg.at<uchar>(rowTopR,colTopR));
   pixMat[1] = pixVec;

   rVec = pixMat*rVec;

   	double c = 0;

	for (int i = 0; i != rVec.size(); ++i)
	{
		c += lVec[i]*rVec[i];
	}

   return c;
#else
   return (static_cast<double>(ipImg.at<uchar>(rowBotL,colBotL)) + \
           static_cast<double>(ipImg.at<uchar>(rowBotR,colBotR)) + \
           static_cast<double>(ipImg.at<uchar>(rowTopL,colTopL)) + \
           static_cast<double>(ipImg.at<uchar>(rowTopR,colTopR)))/4;
#endif
}

#endif //INTERPPIX_HPP
