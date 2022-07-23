// State.cpp

#include "State.h"

#include <iostream>

State::State(unordered_map<string, float> deltas) : deltas(deltas)
{

}

State& State::operator=(const State& state1)
{
	deltas = state1.deltas;
	return *this;
}

void State::AddDelta(string deltaName, float delta)
{
	if (deltas.find(deltaName) == deltas.end())
	{
		deltas.insert(make_pair(deltaName, delta));
	}
	else
	{
		cerr << "State::AddDelta(): " << deltaName << " delta already exists." << endl;
	}
}

void State::RemoveDelta(string deltaName)
{
	if (deltas.find(deltaName) != deltas.end())
	{
		deltas.erase(deltaName);
	}
	else
	{
		cerr << "State::RemoveDelta(): " << deltaName << " delta doesn't exists." << endl;
	}
}

void State::UpdateDelta(string deltaName, float newDelta)
{
	try 
	{
		deltas.at(deltaName) = newDelta;
	}
	catch (const std::out_of_range& e)
	{
		cerr << "State::UpdateDelta(): " << deltaName << " delta doesn't exists." << endl;
	}
}

void State::Reset()
{
	deltas.clear();
}

string State::ToString() const
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

ostream& operator<<(ostream& os, const State& state)
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
