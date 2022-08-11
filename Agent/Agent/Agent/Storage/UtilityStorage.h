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
	void Save(GameAgent::State newState, float utility);

	/*
	
	*/
	void Load();

	/*
	
	*/
	void Clear();

	/*
	
	*/
	inline std::unordered_map<GameAgent::State, float> GetCachedStates() const { return states; }

private:

	/* */
	std::unordered_map<GameAgent::State, float> states;

	/* */
	bool isLoaded;

};