#include <torch/torch.h>
#include <iostream>

#include "Environment/Environment.h"
#include "Agent.h"

using namespace std;

int main()
{
	torch::Tensor tensor = torch::eye(9);
	cout << tensor << endl;

	Environment<int> env;
	env.AddActions(9);

	return 0;
}
