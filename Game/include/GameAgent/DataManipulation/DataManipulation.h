#pragma once

#include <torch/torch.h>

namespace GameAgent
{
namespace DataManip
{

/* 
  DataManipulation

  A abstract class that defines the variables and functions needed for a
  data manipulation object.

  Author: Darian G.
*/
class DataManipulation
{
protected:

	/*
	  Default constructor.
	*/
	DataManipulation() = default;

public:

	/* 
	  Modifies the inputed data.
	  params:
			- data: The data to be modified
	*/
	virtual torch::Tensor ModifyData(torch::Tensor data) = 0;

};


} /* namespace DataManip */
} /* namespace GameAgent */
