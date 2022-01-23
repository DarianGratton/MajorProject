// State.cpp

#include "State.h"

#include <iostream>

State::State() 
{

}

State::State(map<string, float> deltas) : deltas(deltas)
{

}

void State::AddDelta(pair<string, float> delta)
{
	deltas.insert(delta);
}

void State::RemoveDelta(string deltaName) 
{
	deltas.erase(deltaName);
}

void State::UpdateDelta(string deltaName, float newDelta)
{
	deltas.at(deltaName) = newDelta;
}

void State::Reset()
{
	deltas.clear();
}

string State::ToString() const 
{
	// Create string
	stringstream str;
	str << "\nState's deltas: \n";
	for (pair<string, float> delta : deltas)
	{
		str << delta.first << ": " << to_string(delta.second) << "\n";
	}
	return str.str();
}

vector<float> State::ToVector() const
{
	// Copy deltas to vector
	vector<float> values;
	for (pair<string, float> delta : deltas)
	{
		values.push_back(delta.second);
	}

	return values;
}


torch::Tensor State::ToTensor() const
{
	// Copy deltas to vector
	vector<float> values = ToVector();

	// Create and return the Tensor
	// TODO: Test
	int64_t vec_size = values.size();
	torch::TensorOptions opts = torch::TensorOptions().dtype(torch::kFloat32);
	return torch::from_blob(values.data(), { vec_size }, opts).clone();
}
