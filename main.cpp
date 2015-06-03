#include <vector>
#include "Matrix.hpp"
#include "BP.hpp"

int main(int argc, char* argv[])
{
  cv::Mat lImg = cv::imread(argv[1],0); //left image
  cv::Mat rImg = cv::imread(argv[2],0); //right image
	
  //cv::imwrite("left_image.png", lImg);
  //cv::imwrite("right_image.png", rImg);

  //cv::imshow("test",lImg);
  //cv::waitKey();

  int baseDist = atoi(argv[3]); //baseline distance

  BP myBP(lImg, rImg, baseDist); //compute 255 levels depth for stereo pair (note: a level corresponds to single pixel shift) (note: 255 makes things very slow)

  myBP.loopyBPIterate();

  return 0;
}
