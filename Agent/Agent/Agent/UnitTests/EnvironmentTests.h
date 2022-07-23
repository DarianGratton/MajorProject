#include <gtest/gtest.h>

#include <vector>
#include <string>
#include <map>

#include "../Environment/Environment.h"

using namespace std;

TEST(EnvUpdate, ValidValues)
{
	State state;
	state.AddDelta("Delta 1", 1.0f);
	state.AddDelta("Delta 2", 2.0f);
	
	Environment env(state);
	vector<float> action = { 1.0f };
	float reward = 25.0f;
	state.UpdateDelta("Delta 2", 5.0f);
	bool terminal = true;

	env.Update(action, reward, state, terminal);
	EXPECT_EQ(env.GetAction()[0], action[0]);
	EXPECT_EQ(env.GetReward(), reward);
	EXPECT_EQ(env.GetCurrState().GetDeltas().at("Delta 2"), 5.0f);
	EXPECT_EQ(env.GetTotalReward(), reward);
	EXPECT_TRUE(env.IsTerminal());
}

TEST(EnvUpdate, ValidValuesNoTerminalValue)
{
	State state;
	state.AddDelta("Delta 1", 1.0f);
	state.AddDelta("Delta 2", 2.0f);

	Environment env(state);
	vector<float> action = { 1.0f };
	float reward = 25.0f;
	state.UpdateDelta("Delta 2", 5.0f);

	env.Update(action, reward, state);
	EXPECT_EQ(env.GetAction()[0], action[0]);
	EXPECT_EQ(env.GetReward(), reward);
	EXPECT_EQ(env.GetCurrState().GetDeltas().at("Delta 2"), 5.0f);
	EXPECT_EQ(env.GetTotalReward(), reward);
	EXPECT_FALSE(env.IsTerminal());
}

TEST(EnvReset, ValidValuesNoTerminalValue)
{
	State state;
	state.AddDelta("Delta 1", 1.0f);
	state.AddDelta("Delta 2", 2.0f);

	Environment env(state);
	vector<float> action = { 1.0f };
	float reward = 25.0f;
	state.UpdateDelta("Delta 2", 5.0f);
	env.Update(action, reward, state);
	env.Reset();
	EXPECT_EQ(env.GetAction().size(), 0);
	EXPECT_EQ(env.GetReward(), 0);
	EXPECT_EQ(env.GetCurrState().GetDeltas().at("Delta 1"), 1.0f);
	EXPECT_EQ(env.GetTotalReward(), 0);
	EXPECT_FALSE(env.IsTerminal());
}
