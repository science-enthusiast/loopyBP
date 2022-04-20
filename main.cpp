#include <vector>

#include "BP.hpp"

int main(int argc, char* argv[])
{
  cv::Mat lImg = cv::imread(argv[1],0); //left image
  cv::Mat rImg = cv::imread(argv[2],0); //right image

  //cv::imshow("test",lImg);
  //cv::waitKey();

  int baseDist = atoi(argv[3]); //baseline distance

  BP myBP(lImg, rImg, baseDist);

  myBP.loopyBPIterate();

  return 0;
}
