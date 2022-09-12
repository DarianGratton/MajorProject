// State.cpp

#include "State.h"

#include <iostream>

namespace GameAgent
{ 

State::State(std::unordered_map<std::string, float> deltas) : deltas(deltas)
{

}

State& State::operator=(const State& state1)
{
	deltas = state1.deltas;
	return *this;
}

void State::AddDelta(std::string deltaName, float delta /* = 0.0f */)
{
	if (deltas.find(deltaName) == deltas.end())
	{
		deltas.insert(std::make_pair(deltaName, delta));
	}
	else
	{
		std::cerr << "State::AddDelta(): " << deltaName << " delta already exists." << std::endl;
	}
}

void State::RemoveDelta(std::string deltaName)
{
	if (deltas.find(deltaName) != deltas.end())
	{
		deltas.erase(deltaName);
	}
	else
	{
		std::cerr << "State::RemoveDelta(): " << deltaName << " delta doesn't exists." << std::endl;
	}
}

void State::UpdateDelta(std::string deltaName, float newDelta)
{
	try 
	{
		deltas.at(deltaName) = newDelta;
	}
	catch (const std::out_of_range& e)
	{
		std::cerr << "State::UpdateDelta(): " << deltaName << " delta doesn't exists." << std::endl;
	}
}

float State::GetDelta(std::string deltaName)
{
	try
	{
		return deltas.at(deltaName);
	}
	catch (const std::out_of_range& e)
	{
		std::cerr << "State::GetDelta(): " << deltaName << " delta doesn't exists." << std::endl;
		throw e;
	}
}

void State::Reset()
{
	deltas.clear();
}

bool State::Contains(std::string deltaName, float delta) const
{
	return deltas.find(deltaName) != deltas.end() && deltas.at(deltaName) == delta;
}

std::string State::ToString() const
{
	// Create string
	std::stringstream str;
	str << "State's deltas: \n";
	for (std::pair<std::string, float> delta : deltas)
	{
		str << delta.first << ": " << std::to_string(delta.second) << "\n";
	}
	return str.str();
}

std::vector<float> State::ToVector() const
{
	// Copy deltas to vector
	std::vector<float> values;
	for (std::pair<std::string, float> delta : deltas)
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
	for (std::pair<std::string, float> delta : deltas)
	{
		tensor[0][i].data() = delta.second;
		i++;
	}

	return tensor.clone();
}

std::ostream& operator<<(std::ostream& os, const State& state)
{
	unsigned int i = 0;
	os << "State's deltas: \n";
	for (std::pair<std::string, float> delta : state.deltas)
	{
		if (i == state.deltas.size() - 1)
			os << delta.first << ": " << std::to_string(delta.second);
		else
			os << delta.first << ": " << std::to_string(delta.second) << "\n";
		i++;
	}
	return os;
}

bool operator==(const State& lhs, const State& rhs)
{
	if (lhs.deltas.size() != rhs.deltas.size())
		return false;

	for (auto delta : lhs.deltas)
	{
		if (rhs.deltas.find(delta.first) != rhs.deltas.end())
		{
			if (rhs.deltas.at(delta.first) != lhs.deltas.at(delta.first))
			{
				return false;
			}
		}
		else
		{
			return false;
		}
	}
	return true;
}

} /* namespace GameAgent */
