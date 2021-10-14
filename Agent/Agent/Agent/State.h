#pragma once

#include <torch/torch.h>

#include <vector>
#include <string>
#include <map>

using namespace std;

/// TODO: Copy Constuctor
class State {
public:
	/// <summary>
	/// Default constructor that constructs an state object with no deltas.
	/// </summary>
	State();

	/// <summary>
	/// Constructor that creates an state object with already defined deltas.
	/// </summary>
	/// <param name="deltas"></param>
	State(map<string, long> deltas);

	/// <summary>
	///	Copy constructor that constructs an copy of another state object.
	/// </summary>
	/// <param name="">Another state object to copy.</param>
	State(const State&) {}

	/// <summary>
	/// Assignment operator for copying another state object.
	/// </summary>
	/// <param name="reward">Another state object to copy.</param>
	/// <returns></returns>
	State& operator=(const State& state) { return *this; }


	/// <summary>
	/// Add new delta to the exist list of deltas.
	/// </summary>
	/// <param name="delta">The delta to be added.</param>
	void AddDelta(pair<string, long> delta);

	/// <summary>
	/// Removes an delta that exists in the list of deltas.
	/// </summary>
	/// <param name="deltaName">The delta to remove.</param>
	void RemoveDelta(string deltaName);
	
	/// <summary>
	/// Updates the value of an delta that exists in the list of deltas.
	/// </summary>
	/// <param name="deltaName">The delta to change.</param>
	/// <param name="newDelta">The new value to change the delta to.</param>
	void UpdateDelta(string deltaName, long newDelta);
	
	/// <summary>
	/// Resets the state by removing all the stored deltas.
	/// </summary>
	void Reset();
	
	/// <summary>
	/// Helper function for return the state's delta as an readable string. Useful for debugging.
	/// </summary>
	/// <returns>An formatted string.</returns>
	string ToString() const;

	/// <summary>
	/// Helper function for getting the state's deltas as an tensor object.
	/// </summary>
	/// <returns>An tensor object of the state's deltas.</returns>
	torch::Tensor ToTensor() const;

	/// <summary>
	/// Gets an copy of delta's data structure.
	/// </summary>
	/// <returns>The deltas.</returns>
	inline map<string, long> GetDeltas() { return deltas; }

private:
	/// <summary>
	/// An map that stores the state's deltas. Uses an string (the name of delta) for the key.
	/// Example delta: PlayerHp, 40 
	/// </summary>
	map<string, long> deltas;
};
