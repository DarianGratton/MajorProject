#pragma once

#include <torch/torch.h>

// TODO: Implement functionality for a basic SACAgent
class SACAgent 
{
public:

	SACAgent();

	void ChooseAction();
	void UpdateMemory();
	void UpdateNetworkParameters();
	void Learn();

	void SaveModel();
	void LoadModel();

};
