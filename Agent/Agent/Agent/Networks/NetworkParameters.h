#pragma once

#include <string>

class NetworkParameters
{
public:
	/* 
	  Gets the name of the network. 
	*/
	virtual inline std::string GetNetworkName() const = 0;
};