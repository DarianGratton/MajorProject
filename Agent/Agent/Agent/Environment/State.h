#pragma once

#include <torch/torch.h>

#include <vector>
#include <string>
#include <map>



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
		
	/* 
	  Default Constructor.
	*/
	State() = default;

	/* 
	  Constructor for taking a pre-defined set of deltas. 
	  params: 
			- deltas: A map of deltas to set the state to.
	*/
	State(std::unordered_map<std::string, float> deltas);

	/* 
	  Copy Constructor.
	*/
	State(const State& state1) { deltas = state1.deltas; }

	/* 
	  Assignment Operator.
	*/
	State& operator=(const State& state1);

	/*
	  Adds a new delta to the list of existing list of deltas.
	  params:
			- delta: A new delta to add.
	*/
	void AddDelta(std::string deltaName, float delta);

	/*
	  Removes an existing delta.
	  params:
			- deltaName: The name of the delta to be removed.
	*/
	void RemoveDelta(std::string deltaName);

	/*
	  Updates an existing delta to a new value.
	  params:
			- deltaName: The name of the delta to update.
			- newDelta: The value of the new delta.
	*/
	void UpdateDelta(std::string deltaName, float newDelta);

	/* 
	  Resets the state by removing all the stored deltas. 
	*/
	void Reset();

	/* 
	  Returns the size of the state. 
	*/
	inline int64_t Size() { return deltas.size(); }

	/* 
	  Returns a copy of the state's deltas. 
	*/
	inline std::unordered_map<std::string, float> GetDeltas() { return deltas; }

	/* 
	  Helper function for changing the state to a std::vector object. 
	  returns:
			- A newly created vector containing the state's deltas.
	*/
	std::vector<float> ToVector() const;

	/* 
	  Helper function for changing the state to a torch::Tensor object. 
	  Returned dimension of { 1, state.size() }. 
	  returns:
			- A newly created torch::Tensor containing the state's deltas.
	*/
	torch::Tensor ToTensor() const;

	/* 
	  Converts the state into a readable string.
	  returns:
			- The state's deltas as a string
	*/
	std::string ToString() const;

	/* 
	  Insertion Operator. 
	*/
	friend std::ostream& operator<<(std::ostream& os, const State& state);

private:

	/* 
	  An map that stores the state's deltas. Uses an string (name of delta) for the key.

	  Example Deltas:
	    - X: 90.0
		- Y: -120.0
		- Z: 5.0
	*/
	std::unordered_map<std::string, float> deltas;
};
