#ifndef BIRCHTOMASDISP_HPP
#define BIRCHTOMASDISP_HPP
//
// Birchfield-Tomasi measure that is insensitive to image sampling
//
#include <math.h>
//#include <cv.h>

std::vector<double> birchTomasDisp(int, int, int, const cv::Mat&, const cv::Mat&);

#endif //BIRCHTOMASDISP_HPP
