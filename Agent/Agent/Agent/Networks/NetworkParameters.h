#pragma once

#include <string>

namespace GameAgent 
{
namespace Networks 
{

/* 
  NetworkParameters

  A abstract class that defines the variables and functions needed for a 
  network parameters object.

  Author: Darian G.
*/
class NetworkParameters
{
public:
	/* 
	  Gets the name of the network. 
	*/
	virtual inline std::string GetNetworkName() const = 0;
};

} /* namespace Networks */
} /* namespace GameAgent */
