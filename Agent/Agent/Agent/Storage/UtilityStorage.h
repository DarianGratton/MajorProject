#pragma once

#include <map>
#include <string>
#include <vector>

#include "../Environment/State.h"

class UtilityStorage
{
public:

	/*
	  Default Constructor.
	*/
	UtilityStorage();

	/*
	
	*/
	void Save(GameAgent::State newState, float utility, std::string filename = "UtilityStorage.txt");

	/*
	
	*/
	std::vector<std::pair<GameAgent::State, float>> Load();

	/*
	
	*/
	void Clear();

	/*
	
	*/
	inline std::vector<std::pair<GameAgent::State, float>> GetCachedStates() const { return cachedStates; }

private:

	/* */
	std::vector<std::pair<GameAgent::State, float>> cachedStates;

	/* */
	bool isLoaded;

};