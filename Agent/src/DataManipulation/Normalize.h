#pragma once

#include "DataManipulation.h"

namespace GameAgent
{
namespace DataManip
{


class Normalize : public DataManipulation
{
public: 

	/* 
	  Main Constructor
	  params:
			- p: The exponent value.
			- dim: The dimension to reduce.
			- eps: Small value to avoid division by zero.
	*/
	Normalize(double p = 2.0, int64_t dim = 1, double eps = 1e-12);

	/* 
	  Normalizes the inputed data.
	  params:
			- data: The tensor to be normalized.
	  returns:
			- The normalized data.
	*/
	torch::Tensor ModifyData(torch::Tensor data) override;

private:

	/* The exponent value. */
	double p;

	/* The dimension to reduce. */
	int64_t dim;

	/* Small value to avoid divsion by zero. */
	double eps; 

};


} /* namespace DataManip */
} /* namespace GameAgent */
