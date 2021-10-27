#pragma once

#include <torch/torch.h>

// TODO: Implement functionality for a basic SACAgent
class ReplayMemory
{
public:

	ReplayMemory();

	void StoreStateTransition();
	void SampleMemory();
	
	void SaveMemory();
	void LoadMemory();

private:
	unsigned int maxMemSize;
	unsigned int memCounter;
	int stateMem;	 // Placeholder type
	int newStateMem; // Placeholder type
	int actionMem;	 // Placeholder type
	int rewardMem;	 // Placeholder type
	int terminalMem; // Placeholder type

};
