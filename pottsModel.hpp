#ifndef POTTSMODEL_HPP
#define POTTSMODEL_HPP

#include <math.h>
#include <vector>

#include <opencv.hpp>
#ifdef __linux__
#include <core/core.hpp>
#include <highgui/highgui.hpp>
#elif _WIN32
#include <core\core.hpp>
#include <highgui\highgui.hpp>
#endif

std::vector<std::vector<double> > pottsModel(int rPos, int cPos, int rOff, int cOff, int nLabel, const cv::Mat& ipImg)
{
  std::vector<double> smoothVec;
  std::vector<std::vector<double> > smoothMat;
  //Potts model parameters: values according to original graph cuts paper
  int K = 50; //penalty for violating smoothness constraint (i.e., adjacent labels have same value)
  int P = 2; //extra penalty if the pixels have small difference in value
  int diffThresh = 4; //threshold on pixel value difference

  for (int lP = 0; lP < nLabel; ++lP)
  {
    for (int lQ = 0; lQ < nLabel; ++lQ)
    {
      double iDiff = std::abs(ipImg.at<uchar>(rPos, cPos) - ipImg.at<uchar>(rPos + rOff, cPos + cOff));
      int condInd = 0;
      if (lP != lQ)
      {
        condInd = 1;
      }

      if (iDiff < diffThresh)
      {
        smoothVec.push_back(P*K*condInd);
      }
      else
      {
        smoothVec.push_back(K*condInd);
      }
    }
    smoothMat.push_back(smoothVec);
    smoothVec.clear();
  }

  return smoothMat;
}

#endif //POTTSMODEL_HPP
