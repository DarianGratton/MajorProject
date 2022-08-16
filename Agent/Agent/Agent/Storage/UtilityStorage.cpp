#include "UtilityStorage.h"

#include <fstream>
#include <sstream>
#include <vector>

UtilityStorage::UtilityStorage()
{
}

void UtilityStorage::Save(GameAgent::State newState, float utility, std::string filename)
{
	// Check for dupicate
	bool foundDup = false;
	auto storedStates = Load();
	for (int i = 0; i < storedStates.size(); i++)
	{
		if (storedStates.at(i).first == newState)
		{
			storedStates.at(i).second = utility;
			foundDup = true;
			break;
		}
	}

	if (!foundDup)
		storedStates.push_back(std::make_pair(newState, utility));

	// Create string
	std::stringstream str;
	for (int i = 0; i < storedStates.size(); i++)
	{
		for (auto delta : storedStates.at(i).first.GetDeltas())
		{
			str << delta.first << " " << std::to_string(delta.second) << " ";
		}
		str << std::to_string(storedStates.at(i).second) << " " << std::endl;
	}

	// Save to end of file
	std::ofstream ofs;
	ofs.open("Storage.txt", std::ifstream::out | std::ifstream::trunc);
	ofs << str.rdbuf();
	ofs.close();
}

std::vector<std::pair<GameAgent::State, float>> UtilityStorage::Load()
{
	// Open stream
	std::ifstream ifs;
	ifs.open("Storage.txt", std::ifstream::out);

	// Parse file
	std::string line;
	std::vector<std::pair<GameAgent::State, float>> storage;
	while (std::getline(ifs, line))
	{
		// Split file contents
		size_t pos;
		std::vector<std::string> data;
		std::string token;
		while ((pos = line.find(" ")) != std::string::npos)
		{
			token = line.substr(0, pos);
			line.erase(0, pos + 1);
			data.push_back(token);
		}

		// Create state and utility
		GameAgent::State state; 
		for (int i = 0; i < data.size() - 1; i = i + 2)
		{
			int j = i + 1;
			state.AddDelta(data.at(i), std::stof(data.at(j)));
		}
		float utility = std::stof(data.at(data.size() - 1));

		// Push to storage
		storage.push_back(std::make_pair(state, utility));
	}

	ifs.close();
	return storage;
}

std::vector<std::pair<GameAgent::State, float>> UtilityStorage::Search(GameAgent::State state)
{
	std::vector<std::pair<GameAgent::State, float>> states;
	if (state.IsEmpty())
		return states;

	// Check storage for key and values 
	auto storedStates = Load();
	bool foundDifferingKeyValue = false;
	for (int i = 0; i < storedStates.size(); i++)
	{
		for (auto delta : state.GetDeltas())
		{
			if (!storedStates.at(i).first.Contains(delta.first, delta.second))
			{
				foundDifferingKeyValue = true;
				break;
			}
		}

		if (!foundDifferingKeyValue)
			states.push_back(storedStates.at(i));
	}

	return states;
}

void UtilityStorage::Clear()
{
	std::ofstream ofs;
	ofs.open("Storage.txt", std::ofstream::out | std::ofstream::trunc);
	ofs.close();
}
