#include <torch/torch.h>
#include <iostream>
#include <sstream>
#include <vector>

#include "Environment/Environment.h"
#include "Environment/State.h"
#include "Agent.h"

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

	return 0;
}
