#pragma once

/// <summary>
/// 
/// </summary>
/// <typeparam name="T"></typeparam>
/// TODO: Type T should be restricted to number types
template <class T>
class Reward {
public:
	/// <summary>
	/// Default constructor that constructs an reward object.
	/// </summary>
	Reward() {}

	/// <summary>
	/// Constructs and initializes an reward object.
	/// </summary>
	/// <param name="reward">The reward.</param>
	Reward(T reward);

	/// <summary>
	/// Copy constructor that constructs an copy of another reward object.
	/// </summary>
	/// <param name="">Another reward object to copy.</param>
	Reward(const Reward&) {}

	/// <summary>
	/// Assignment operator for copying another reward object.
	/// </summary>
	/// <param name="reward">Another reward object to copy.</param>
	/// <returns></returns>
	Reward& operator=(const Reward& reward) { return *this; }

	/// <summary>
	/// Sets the reward.
	/// </summary>
	/// <param name="newReward">The new reward.</param>
	inline void SetReward(T newReward) { reward = newReward; }

	/// <summary>
	/// Gets the reward.
	/// </summary>
	/// <returns>The reward</returns>
	inline T GetReward() { return reward; }

private:
	/// <summary>
	/// The reward variable that stores the value of the reward.
	/// </summary>
	T reward;
};
