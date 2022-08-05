#pragma once

#include <string>

using namespace std;

class NetworkParameters
{
public:
	/* 
	  Gets the name of the network. 
	*/
	virtual inline string GetNetworkName() const = 0;
};