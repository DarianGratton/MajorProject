#pragma once

#include <torch/torch.h>

#include <vector>
#include <string>
#include <map>

using namespace std;

namespace RLGameAgent 
{

	/*
	  State
	
	  A class that defines what constitutes a state in the environment. Contains a
	  map that stores deltas that can include variables such as player position,
	  enemy position, weapon position, etc. Also contains functions for manipulating 
	  said state.
	  
	  Author: Darian G.
	*/
	class State 
	{
	public:
		
		/* Default Constructor */
		State();

		/* 
		  Constructor for taking a pre-defined set of deltas. 
		  params: 
				- deltas: A map of deltas to set the state to
		*/
		State(unordered_map<string, float> deltas);

		/* Copy Constructor */
		State(const State& state1) { deltas = state1.deltas; }

		/* Assignment Operator */
		State& operator=(const State& state1);

		/*
		  Adds a new delta to the list of existing list of deltas.
		  params:
				- delta: A new delta to add.
		*/
		void AddDelta(pair<string, float> delta);

		/*
		  Removes an existing delta.
		  params:
				- deltaName: The name of the delta to be removed.
		*/
		void RemoveDelta(string deltaName);

		/*
		  Updates an existing delta to a new value.
		  params:
				- deltaName: The name of the delta to update.
				- newDelta: The value of the new delta.

		  TODO: Should probably have some error handling if deltaName does not exist.
		*/
		void UpdateDelta(string deltaName, float newDelta);

		/* 
		  Resets the state by removing all the stored deltas. 
		*/
		void Reset();

		/* Returns the size of the state. */
		inline int64_t Size() { return deltas.size(); }

		/* Returns a copy of the state's deltas. */
		inline unordered_map<string, float> GetDeltas() { return deltas; }

		/* Helper function for changing the state to a std::vector object. */
		vector<float> ToVector() const;

		/* 
		  Helper function for changing the state to a torch::Tensor object. 
		  Returned dimension of { 1, state.size() }. 
		*/
		torch::Tensor ToTensor() const;

		/* 
		  Converts the state into a readable string. 
		  TODO: String formatting.
		*/
		string ToString() const;

		/* Insertion Operator. */
		friend ostream& operator<<(ostream& os, const State& state);

	private:

		/* 
		  An map that stores the state's deltas. Uses an string (name of delta) for the key.

		  Example Deltas:
			- X: 90.0
			- Y: -120.0
			- Z: 5.0
		*/
		unordered_map<string, float> deltas;
	};

}
