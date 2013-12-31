#include <vector>
#include "Matrix.hpp"
#include "BP.hpp"

int main(int argc, char* argv[])
{
	std::vector<double> b;
	std::vector< std::vector<double> > A;

    for (int i = 1; i <= 4; ++i)
	{
		b.push_back(i);
	}

	std::vector<double> a;
	a.push_back(1); a.push_back(0); a.push_back(0); a.push_back(0);
	A.push_back(a);
	a.clear();
	a.push_back(0); a.push_back(1); a.push_back(0); a.push_back(0);
	A.push_back(a);
	a.clear();
	a.push_back(0); a.push_back(0); a.push_back(1); a.push_back(0);
	A.push_back(a);
	a.clear();
	a.push_back(0); a.push_back(0); a.push_back(0); a.push_back(1);
	A.push_back(a);

	std::vector<double> c = A*b; //Matrix::mvMultiply(A,b);

	double d = 0;

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