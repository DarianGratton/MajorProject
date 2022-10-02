#pragma once

#include "DataManipulation.h"

namespace GameAgent
{
namespace DataManip
{


class Normalize : public DataManipulation
{
public: 

	/* Constructor */
	Normalize(double p = 2.0, int64_t dim = 1, double eps = 1e-12);

	torch::Tensor ModifyData(torch::Tensor data) override;

private:

	/* The exponent value. */
	double p;

	/* The dimension to reduce. */
	int64_t dim;

	/* Small value to avoid divsion by zero */
	double eps; 

};


} /* namespace DataManip */
} /* namespace GameAgent */
