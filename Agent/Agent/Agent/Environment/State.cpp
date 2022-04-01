// State.cpp

#include "State.h"

#include <iostream>

RLGameAgent::State::State() 
{

}

RLGameAgent::State::State(unordered_map<string, float> deltas) : deltas(deltas)
{

}

RLGameAgent::State& RLGameAgent::State::operator=(const State& state1)
{
	deltas = state1.deltas;
	return *this;
}

void RLGameAgent::State::AddDelta(pair<string, float> delta)
{
	deltas.insert(delta);
}

void RLGameAgent::State::RemoveDelta(string deltaName)
{
	deltas.erase(deltaName);
}

void RLGameAgent::State::UpdateDelta(string deltaName, float newDelta)
{
	deltas.at(deltaName) = newDelta;
}

void RLGameAgent::State::Reset()
{
	deltas.clear();
}

string RLGameAgent::State::ToString() const
{
	// Create string
	stringstream str;
	str << "State's deltas: \n";
	for (pair<string, float> delta : deltas)
	{
		str << delta.first << ": " << to_string(delta.second) << "\n";
	}
	return str.str();
}

vector<float> RLGameAgent::State::ToVector() const
{
	// Copy deltas to vector
	vector<float> values;
	for (pair<string, float> delta : deltas)
	{
		values.push_back(delta.second);
	}

	return values;
}

torch::Tensor RLGameAgent::State::ToTensor() const
{
	// Create tensor
	int64_t vec_size = ToVector().size();
	torch::TensorOptions opts = torch::TensorOptions().dtype(torch::kFloat32);
	torch::Tensor tensor = torch::zeros({ 1, vec_size }, opts);
	
	// Store values
	int i = 0;
	for (pair<string, float> delta : deltas)
	{
		tensor[0][i].data() = delta.second;
		i++;
	}

	return tensor.clone();
}

ostream& RLGameAgent::operator<<(ostream& os, const State& state)
{
	unsigned int i = 0;
	os << "State's deltas: \n";
	for (pair<string, float> delta : state.deltas)
	{
		if (i == state.deltas.size() - 1)
			os << delta.first << ": " << to_string(delta.second);
		else
			os << delta.first << ": " << to_string(delta.second) << "\n";
		i++;
	}
	return os;
}
