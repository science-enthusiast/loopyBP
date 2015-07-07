//
// Birchfield-Tomasi measure that is insensitive to image sampling
//
#include <math.h>
//#include <cv.h>
#include "interpPix.hpp"
#include "birchTomasDisp.hpp"

#if 0 //based on the interpretation in original graph cuts paper
std::vector<double> birchTomasDisp(int rPos, int cPos, int nLabel, const cv::Mat& lImg, const cv::Mat& rImg)
{
   double constThresh = 20;

   std::vector<double> dataVec;

   int iL = 0;

   while ((iL < nLabel) && ((cPos + iL) < rImg.cols))
   {
	  double minFwd = std::abs(rImg.at<uchar>(rPos, cPos) - interpPix(rPos, cPos + iL - 0.5, lImg));
      double minRev = std::abs(interpPix(rPos, cPos - 0.5, rImg) - lImg.at<uchar>(rPos, cPos + iL));
      for (double deltaX = -0.25; deltaX != 0.5; deltaX += 0.25)
      {
         double fwd = std::abs(rImg.at<uchar>(rPos, cPos) - interpPix(rPos, cPos + iL + deltaX, lImg));
         double rev = std::abs(interpPix(rPos, cPos + deltaX, rImg) - lImg.at<uchar>(rPos, cPos + iL));
         if (fwd < minFwd)
         {
            minFwd = fwd;
         }
         if (rev < minRev)
         {
            minRev = rev;
         }
      }
      if (minFwd <= minRev)
      {
         if (minFwd < constThresh)
         {
            dataVec.push_back(minFwd);
         }
         else
         {
            dataVec.push_back(constThresh);
         }
      }
      else if (minRev < constThresh)
      {
         dataVec.push_back(minRev);
      }
      else
      {
         dataVec.push_back(constThresh);
      }

	  ++iL;
   }

   int extraSz = nLabel - dataVec.size();

   if (extraSz != 0)
   {
	   double dataBuff = dataVec.back();
	   for (int i = 0; i != extraSz; ++i)
	   {
		   dataVec.push_back(dataBuff);
	   }
   }
  
   return dataVec;
}
#else //based on the steps given in Birchfield PAMI paper
std::vector<double> birchTomasDisp(int rPos, int cPos, int nLabel, const cv::Mat& lImg, const cv::Mat& rImg)
{
  std::vector<double> dataVec;

  int iL = 0;

  int nCol = rImg.cols;
  int nRow = rImg.rows;

  while ((iL < nLabel) && ((cPos + iL) < nCol)) {
   std::vector<double> rVal;

   rVal.push_back(static_cast<double>(rImg.at<uchar>(rPos, cPos)));

   if (cPos < nCol - 1) {
    rVal.push_back((static_cast<double>(rImg.at<uchar>(rPos, cPos)) + static_cast<double>(rImg.at<uchar>(rPos, cPos+1)))/2);
   }
   else {
    rVal.push_back(static_cast<double>(rImg.at<uchar>(rPos, cPos)));
   }

   if (cPos > 0) {
    rVal.push_back((static_cast<double>(rImg.at<uchar>(rPos, cPos)) + static_cast<double>(rImg.at<uchar>(rPos, cPos-1)))/2);
   }
   else {
    rVal.push_back(static_cast<double>(rImg.at<uchar>(rPos, cPos)));
   }

   double Imax = *std::max_element(rVal.begin(), rVal.end());
   double Imin = *std::min_element(rVal.begin(), rVal.end());

   double lVal;
	  
   if (cPos + iL < 0) {
    lVal = static_cast<double>(lImg.at<uchar>(rPos, cPos));
   }
   else {
    lVal = static_cast<double>(lImg.at<uchar>(rPos, cPos + iL));
   }

   std::vector<double> opSelect;

   opSelect.push_back(lVal - Imax);
   opSelect.push_back(Imin - lVal);
   opSelect.push_back(0);

   dataVec.push_back(*std::max_element(opSelect.begin(), opSelect.end()));

   ++iL;
  }

  int extraSz = nLabel - dataVec.size();

  if (extraSz != 0) {
   double dataBuff = dataVec.back();
   for (int i = 0; i != extraSz; ++i) {
    dataVec.push_back(dataBuff);
   }
  }

  return dataVec;
}
#endif
