#pragma once

#include <vector>

#include "State.h"
#include "Reward.h"

using namespace std;

// TODO: Template is a work around for Reward type and variable, should figure out better way
// - Overloaded functions might work with multiple reward variables
/// <summary>
/// 
/// </summary>
/// <typeparam name="T"></typeparam>
template<class T>
class Environment {
public:
	/// <summary>
	/// Default constructor that constructs an environment object.
	/// </summary>
	Environment();

	/// <summary>
	/// Set the current state of the environment.
	/// </summary>
	/// <param name="newState">The new state of the environment.</param>
	void SetCurrState(State& newState);
	
	/// <summary>
	/// Set the returned reward from an action taken in this environment.
	/// </summary>
	/// <param name="newReward"></param>
	void SetReturnedReward(T newReward);
	
	/// <summary>
	/// Add an action to the set of actions. Actions are just 0...n number of actions. 
	/// </summary>
	void AddAction();
	
	/// <summary>
	/// Add an inputted number of actions to the set of actions. Actions are just 0...n number of actions. 
	/// </summary>
	/// <param name="numOfActions">The number of actions to add.</param>
	void AddActions(int numOfActions);

	/// <summary>
	/// Remove an action to the set of actions. Actions are just 0...n number of actions. 
	/// </summary>
	void RemoveAction();
	
	/// <summary>
	/// Remove an inputted number of actions from the set of actions. Actions are just 0...n number of actions. 
	/// </summary>
	/// <param name="numOfActions">The number of actions to remove.</param>
	void RemoveActions(int numOfActions);

	/// <summary>
	/// Gets the current state of the environment.
	/// </summary>
	/// <returns>The current state of the environment.</returns>
	inline State GetCurrState() { return currState; }
	
	/// <summary>
	/// Gets the stored returned reward of an action taken in the environment.
	/// </summary>
	/// <returns>Returned reward of an action taken in the environement.</returns>
	inline Reward<T> GetReturnedReward() { return returnedReward; }
	
	/// <summary>
	/// Gets the data structure of actions. 
	/// </summary>
	/// <returns>The actions.</returns>
	inline vector<long> GetActions() { return actions; }

private:
	/// <summary>
	/// The current state of the environment.
	/// </summary>
	State currState;
	
	/// <summary>
	/// The returned reward of an action taken in the environment.
	/// </summary>
	Reward<T> returnedReward;
	
	/// <summary>
	/// An vector of actions. Is just 0...n number of actions.
	/// </summary>
	vector<long> actions;
};
