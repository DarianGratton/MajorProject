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
	std::vector<std::pair<GameAgent::State, float>> Search(GameAgent::State state);

	/*
	
	*/
	void Clear();

};