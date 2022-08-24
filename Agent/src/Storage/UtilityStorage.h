#pragma once

#include <map>
#include <string>
#include <vector>

#include "../Environment/State.h"

/* 
  UtilityStorage

  A class that handles the storage of the environment's different initial states and their average 
  utilities. Contains functions for saving, loading, and manipulating the states in the file.

  Author: Darian G.
*/
class UtilityStorage
{
public:

	/*
	  Default Constructor.
	*/
	UtilityStorage(std::string filename = "UtilityStorage.txt");

	/*
	  Saves a environment state and its utility to storage. If the state already is contained in 
	  storage, stored utility will be updated. 
	  params:
			- state: The state to be stored.   
			- utility: The utility of that state.s
	*/
	void Save(GameAgent::State state, float utility);

	/*
	  Loads states and utilities from storage.
	  returns:
			 - A vector containing all the states and utilities in storage.
	*/
	std::vector<std::pair<GameAgent::State, float>> Load();

	/*
	  Searchs storage for states that match the deltas in the inputted state.
	  params:
			- state: State containng deltas to search storage for.
	  returns:
			- A vector containing all the states and utilities that match the inputted deltas.
	*/
	std::vector<std::pair<GameAgent::State, float>> Search(GameAgent::State state);

	/*
	  Clears storage of all stored states and utilities.
	*/
	void Clear();

private:

	/* The storage's file name. */
	std::string filename;

};