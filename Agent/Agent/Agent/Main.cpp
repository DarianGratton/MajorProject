#include <torch/torch.h>
#include <iostream>
#include <sstream>
#include <vector>

#include "Environment/Environment.h"
#include "Environment/State.h"
#include "Agent.h"
#include "Networks/SAC/SACAgent.h"
#include "PyTorchExpansion/TorchNormal.h"
#include "Networks/TestEnvironment/AgentTest.h"

using namespace std;

// Used for testing
int main()
{
	//torch::nn::Linear model(3, 4);
	//std::stringstream stream;
	//cout << "Before save: " << endl;
	//// cout << stream.str() << endl << endl;
	//cout << model.get()->weight << endl;
	//// Note that the same stream cannot be used in multiple torch::save(...)
	//// invocations, otherwise the header will be corrupted.
	//torch::save(model, stream);
	//cout << "After save: " << endl;
	//cout << stream.str() << endl << endl;

	//cout << "After clearing stream: " << endl;
	//stream.str(std::string());
	//cout << stream.str() << endl << endl;

	//torch::save(model, stream);
	//cout << "After saving again: " << endl;
	//cout << stream.str() << endl << endl;

	//State state;
	//state.AddDelta(pair<string, float>("One", 0.0f));
	//state.AddDelta(pair<string, float>("Two", 0.0f));
	//state.AddDelta(pair<string, float>("Three", 0.9987f));
	//state.AddDelta(pair<string, float>("Four", -0.0506f));
	//state.AddDelta(pair<string, float>("Five", 0.0f));

	////cout << state.ToString() << endl; 

	//torch::Tensor tensor = state.ToTensor();
	//torch::Tensor tensorTest = torch::tensor({{0.0f, 0.0f, 0.9994f, 0.0337f, 0.0f}});

	//SACAgent agent(0.0003f, 0.0003, 2, 2, 
	//	state.Size(), 256, 256,
	//	1000000, 0.99f, 0.005f, 256, 2);
	//agent.ChooseAction(tensor);

	// AgentTest test;
	// test.Train();

	SACAgent agent(0.0003f, 0.0003, 1, 1,
			5, 256, 256,
			1000000, 0.99f, 0.005f, 256, 2);
	agent.Learn();

	return 0;
}
