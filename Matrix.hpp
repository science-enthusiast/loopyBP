#ifndef MATRIX_HPP
#define MATRIX_HPP
#include <vector>

class Matrix
{
  public:
	static std::vector<double> mvMultiply(std::vector< std::vector<double> > a, std::vector<double> b)
	{
		//throw an exception if dimensions don't match
		std::vector<double> c;
    c.reserve(a.size());

		for (int i = 0; i != a.size(); ++i)
		{
			double d = 0;
			for (int j = 0; j != b.size(); ++j)
			{
				d += a[i][j]*b[j];
			}
			c.push_back(d);
		}
		return c;
	}

	static std::vector<double> vAdd(std::vector<double> a, std::vector<double> b)
	{
		//throw an exception if dimensions don't match
		std::vector<double> c;
		for (int i = 0; i != a.size(); ++i)
		{
			c.push_back(a[i] + b[i]);
		}
		return c; 
	}
};

static std::vector<double> operator* (std::vector< std::vector<double> > A, std::vector<double> b)
{
	//throw an exception if dimensions don't match
	std::vector<double> c;

	for (int i = 0; i != A.size(); ++i)
	{
		double d = 0;
		for (int j = 0; j != b.size(); ++j)
		{
			d += A[i][j]*b[j];
		}
		c.push_back(d);
	}
	return c;
}

static std::vector<double> operator* (std::vector<double> b, std::vector< std::vector<double> > A)
{
	//throw an exception if dimensions don't match
	std::vector<double> c;

	for (int i = 0; i != A[0].size(); ++i)
	{
		double d = 0;
		for (int j = 0; j != b.size(); ++j)
		{
			d += A[j][i]*b[j];
		}
		c.push_back(d);
	}
	return c;
}

static std::vector<double> operator+ (std::vector<double> a, std::vector<double> b)
{
	std::vector<double> c;
	for (int i = 0; i != a.size(); ++i)
	{
		c.push_back(a[i] + b[i]);
	}
	return c;
}

static std::vector<double> operator* (std::vector<double> a, std::vector<double> b)
{
	//throw an exception if dimensions don't match
	std::vector<double> c;

	for (int i = 0; i != a.size(); ++i)
	{
		c.push_back(a[i]*b[i]);
	}
	return c;
}

/*
static double operatorT (std::vector<double> a, std::vector<double> b)
{
	//throw an exception if dimensions don't match
	double c = 0;

	for (int i = 0; i != a.size(); ++i)
	{
		c += a[i]*b[i];
	}
	return c;
}
*/

static std::vector<double> operator/ (std::vector<double> a, double b)
{
	//throw an exception if divide by zero
	std::vector<double> c;

	for (int i = 0; i != a.size(); ++i)
	{
		c.push_back(a[i]/b);
	}
	return c;
}

#endif
