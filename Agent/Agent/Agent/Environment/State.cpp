// State.cpp

#include "State.h"

#include <iostream>

State::State() 
{

}

State::State(map<string, long> deltas) : deltas(deltas) 
{

}

void State::AddDelta(pair<string, long> delta)
{
	deltas.insert(delta);
}

void State::RemoveDelta(string deltaName) 
{
	deltas.erase(deltaName);
}

void State::UpdateDelta(string deltaName, long newDelta)
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
	for (pair<string, long> delta : deltas)
	{
		str << delta.first << ": " << to_string(delta.second) << "\n";
	}
	return str.str();
}

torch::Tensor State::ToTensor() const
{
	// Copy deltas to vector
	vector<long> values;
	for (pair<string, long> delta : deltas)
	{
		values.push_back(delta.second);
	}

	// Create and return the Tensor
	int64_t vec_size = values.size();
	return torch::from_blob(values.data(), { vec_size }).clone();
}
