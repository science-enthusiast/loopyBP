#include <vector>
#include <utility>
#include <map>
#include "Matrix.hpp"
#include "pottsModel.hpp"
#include "birchTomasDisp.hpp"

class BP
{
 private:
  cv::Mat lImg_, rImg_, opMat_;
  std::vector< std::vector<double> > belief;
  std::vector< std::map<int,std::vector<double> > > message;
  std::vector<double> obs;
  std::vector<double> states;
  std::vector< std::vector< std::pair<int, int> > > pixOff;
  std::vector< std::vector<double> > unaryTerm;
  std::map< std::pair<int, int>, std::vector<std::vector<double> > > pairTerm;
  std::vector<uchar> nodePos;
  uchar pixLocation(int row, int col);
  int nRow_, nCol_, nLabel_;
  int maxIter;
  double k;

  int asyncUpdate();
  std::vector<double> computeMessage(int row1, int col1, int row2, int col2);
  int computeBelief(int row, int col);
  int initMessageBelief();
  int initBelief();
  int initMessage();
  int initPos();
  int initFactorTerms();
  int computeUnary();
  int computePair();
 public:
  BP(cv::Mat lImg, cv::Mat rImg, int nLabel);
  int loopyBPIterate();
};

BP::BP(cv::Mat lImg, cv::Mat rImg, int nLabel):nRow_(lImg.rows), nCol_(lImg.cols), nLabel_(nLabel)
{
  lImg_ = lImg;
  rImg_ = rImg;
  opMat_ = lImg;
  std::vector<std::pair<int, int> > pixVec;
  std::pair<int, int> offPair;
  //top left
  offPair = std::make_pair(0,1); pixVec.push_back(offPair);
  offPair = std::make_pair(1,0); pixVec.push_back(offPair);
  pixOff.push_back(pixVec); pixVec.clear();
  //top right
  offPair = std::make_pair(0,-1); pixVec.push_back(offPair);
  offPair = std::make_pair(1,0); pixVec.push_back(offPair);
  pixOff.push_back(pixVec); pixVec.clear();
  //bottom right
  offPair = std::make_pair(0,-1); pixVec.push_back(offPair);
  offPair = std::make_pair(-1,0); pixVec.push_back(offPair);
  pixOff.push_back(pixVec); pixVec.clear();
  //bottom left
  offPair = std::make_pair(-1,0); pixVec.push_back(offPair);
  offPair = std::make_pair(0,1); pixVec.push_back(offPair);
  pixOff.push_back(pixVec); pixVec.clear();
  //top bar
  offPair = std::make_pair(0,-1); pixVec.push_back(offPair);
  offPair = std::make_pair(0,1); pixVec.push_back(offPair);
  offPair = std::make_pair(1,0); pixVec.push_back(offPair);
  pixOff.push_back(pixVec); pixVec.clear();
  //bottom bar
  offPair = std::make_pair(0,-1); pixVec.push_back(offPair);
  offPair = std::make_pair(0,1); pixVec.push_back(offPair);
  offPair = std::make_pair(-1,0); pixVec.push_back(offPair);
  pixOff.push_back(pixVec); pixVec.clear();
  //left bar
  offPair = std::make_pair(-1,0); pixVec.push_back(offPair);
  offPair = std::make_pair(0,1); pixVec.push_back(offPair);
  offPair = std::make_pair(1,0); pixVec.push_back(offPair);
  pixOff.push_back(pixVec); pixVec.clear();
  //right bar
  offPair = std::make_pair(-1,0); pixVec.push_back(offPair);
  offPair = std::make_pair(0,-1); pixVec.push_back(offPair);
  offPair = std::make_pair(1,0); pixVec.push_back(offPair);
  pixOff.push_back(pixVec); pixVec.clear();
  //interior pixels
  offPair = std::make_pair(0,-1); pixVec.push_back(offPair);
  offPair = std::make_pair(0,1); pixVec.push_back(offPair);
  offPair = std::make_pair(-1,0); pixVec.push_back(offPair);
  offPair = std::make_pair(1,0); pixVec.push_back(offPair);
  pixOff.push_back(pixVec); pixVec.clear();

  initFactorTerms();
  initMessageBelief();
  maxIter = 20;
}

int BP::computeBelief(int row, int col)
{
  int node = row*nCol_ + col;
  //int pixOffInd = pixLocation(row, col);
  std::vector<std::pair<int,int> > pixVec = pixOff[nodePos[row*nCol_+col]];
  belief[node] = unaryTerm[node];

  for (int i = 0; i != pixVec.size(); ++i)
  {
    int sendNode = (row+pixVec[i].first)*nCol_ + (col+pixVec[i].second);
    belief[node] = belief[node]*message[sendNode][node];
  }

  double beliefSum = 0;

  for (int i = 0; i != belief[node].size(); ++i)
  {
	  beliefSum += belief[node][i];
  }

  belief[node] = belief[node]/beliefSum;

  return 0;
}

int BP::loopyBPIterate()
{
	for (int i = 0; i != maxIter; ++i)
	{
		asyncUpdate();
		std::cout<<"asynchronous iteration: "<<i<<std::endl;
	}
	
	for (int i = 0; i != nRow_; ++i)
	{
		for (int j = 0; j != nCol_; ++j)
		{
			computeBelief(i,j);

			double depthVal = 0;

			for (int k = 0; k != nLabel_; ++k)
			{
				 depthVal += belief[i*nCol_+j][k]*k; 
			}

			opMat_.at<uchar>(i,j) = static_cast<uchar>((depthVal/static_cast<double>(nLabel_))*255);
		}
	}

	cv::imwrite("depth_image.png",opMat_);

	return 0;
}

//up-down-left-right update; refer tappen & freeman
int BP::asyncUpdate()
{
  for (int i = 0; i != nCol_; ++i)//down all columns
  {
    for (int j = 0; j != (nRow_-1); ++j)
    {
      message[j*nCol_+i][(j+1)*nCol_+i] = computeMessage(j,i,j+1,i);
    }
  }

  for (int i = 0; i != nCol_; ++i)//up all columns
  {
    for (int j = (nRow_-1); j != 0; --j)
    {
      message[j*nCol_+i][(j-1)*nCol_+i] = computeMessage(j,i,j-1,i);
    }
  }

  for (int j = 0; j != nRow_; ++j)//left to right all rows
  {
    for (int i = 0; i != (nCol_-1); ++i)
    {
      message[j*nCol_+i][j*nCol_+i+1] = computeMessage(j,i,j,i+1);
    }
  }

  for (int j = 0; j != nRow_; ++j)//right to left all rows
  {
    for (int i = (nCol_-1); i != 0; --i)
    {
      message[j*nCol_+i][j*nCol_+i-1] = computeMessage(j,i,j,i-1);
    }
  }

  return 0;
}

std::vector<double> BP::computeMessage(int row1, int col1, int row2, int col2)
{
  int nodeTx = row1*nCol_ + col1; //sending node
  int nodeRx = row2*nCol_ + col2; //receiving node

  std::pair<int, int> pairInd;

  if (nodeTx < nodeRx)
  {
    pairInd = std::make_pair(nodeTx, nodeRx);
  }
  else
  {
    pairInd = std::make_pair(nodeRx, nodeTx);
  }

  std::vector<double> mVec = unaryTerm[nodeTx];

  //int pixOffInd = pixLocation(row1,col1);

  std::vector<std::pair<int,int> > pixVec = pixOff[nodePos[row1*nCol_+col1]];

  for (int i = 0; i != pixVec.size(); ++i)
  {
    int neighNode = (row1 + pixVec[i].first)*nCol_ + (col1 + pixVec[i].second);
    if (neighNode != nodeRx)
    {
      mVec = mVec*message[neighNode][nodeTx];
    }
  }

  mVec = pairTerm[pairInd]*mVec;

  double normSum = 0;

  for (int i = 0; i != mVec.size(); ++i)
  {
	normSum += mVec[i];
  }

  mVec = mVec/normSum;

  return mVec;
}

int BP::computeUnary()
{		
  for (int i = 0; i != nRow_; ++i)
  {
    for (int j = 0; j != nCol_; ++j)
    {
      std::vector<double> dispVec = birchTomasDisp(i, j, nLabel_, lImg_, rImg_);

      //double normSum = 0;

      for (int k = 0; k != dispVec.size(); ++k)
      {
	dispVec[k] = exp(-1*(dispVec[k]/16)); //raise to the negative power and normalize to suit BP; according to Tappen and Freeman
	//normSum += dispVec[k];
      }

      //dispVec = dispVec/normSum;

      unaryTerm.push_back(dispVec); //using Birchfield-Tomasi measure
    }
  }

  return 0;
}

int BP::computePair()
{
  for (int iR = 0; iR != nRow_; ++iR)
  {
    for (int iC = 0; iC != nCol_; ++iC)
    {
      //int pixOffInd = pixLocation(iR, iC);

      std::vector<std::pair<int,int> > pixVec = pixOff[nodePos[iR*nCol_ + iC]];

      for (int p = 0; p != pixVec.size(); ++p)
      {
        std::vector<std::vector<double> > pottsMat = pottsModel(iR, iC, pixVec[p].first, pixVec[p].second, nLabel_, lImg_);
    	for (int i = 0; i != pottsMat.size(); ++i)
    	{
	     std::vector<double> pottsVec = pottsMat[i];
	     //double normSum = 0;

         for (int j = 0; j != pottsVec.size(); ++j)
	     {
	      pottsVec[j] = exp(-1*(pottsVec[j]/20)); //raise to the negative power and normalize to suit BP; according to Tappen and Freeman
	      //normSum += pottsVec[j];
	     }
	     pottsMat[i] = pottsVec; ///normSum;
	    }
	    pairTerm.insert(std::make_pair(std::make_pair(iR*nCol_ + iC, (iR+pixVec[p].first)*nCol_ + iC + pixVec[p].second), pottsMat));
      }
    }
  }

  return 0;
}

int BP::initFactorTerms()
{
  initPos();
  computeUnary();
  computePair();

  std::cout<<"done:initialization of energy terms"<<std::endl;

  return 0;
}

int BP::initMessageBelief()
{
  initBelief();
  initMessage();

  return 0;
}

int BP::initBelief()
{
	std::vector<double> initVec(nLabel_, 0);

	for (int i = 0; i != nRow_; ++i)
	{
		for (int j = 0; j != nCol_; ++j)
		{
			belief.push_back(initVec);
		}
	}

	return 0;
}

int BP::initMessage()
{
	std::vector<double> initVec(nLabel_, 1);

	for (int i = 0; i != nRow_; ++i)
	{
		for (int j = 0; j != nCol_; ++j)
		{
		  int nodeTx = i*nCol_ + j; //sending node
		  
		  //int pixOffInd = pixLocation(i,j);

		  std::vector<std::pair<int,int> > pixVec = pixOff[nodePos[i*nCol_+j]];
		  std::map<int, std::vector<double> > messageMap;

		  for (int k = 0; k != pixVec.size(); ++k)
		  {
			int neighNode = (i + pixVec[k].first)*nCol_ + (j + pixVec[k].second);
			messageMap.insert(std::make_pair(neighNode,initVec));
		  }

		  message.push_back(messageMap);
		}
	}

	return 0;
}

int BP::initPos()
{
	for (int i = 0; i != nRow_; ++i)
	{
		for (int j = 0; j != nCol_; ++j)
		{
			nodePos.push_back(pixLocation(i,j));
		}
	}

	return 0;
}

uchar BP::pixLocation(int row, int col)
{
  if ((row == 0) && (col == 0))
  {
    return 0; //top left
  }
  else if ((row == 0) && (col == nCol_-1))
  {
    return 1; //top right 
  }
  else if ((row == nRow_-1) && (col == nCol_-1))
  {
    return 2; //bottom right
  }
  else if ((row == nRow_-1) && (col == 0))
  {
    return 3; //bottom left
  }
  else if ((row == 0) && (col != 0) && (col != nCol_-1))
  {
    return 4; //top bar
  }
  else if ((row == nRow_-1) && (col != 0) && (col != nCol_-1))
  {
    return 5; //bottom bar
  }
  else if (col == 0)
  {
    return 6; //left bar
  }
  else if (col == nCol_-1)
  {
    return 7; //right bar
  }
  else
  {
    return 8; //interior pixels
  }
}
