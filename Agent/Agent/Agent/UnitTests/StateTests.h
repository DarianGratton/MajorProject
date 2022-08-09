#include <gtest/gtest.h>

#include <vector>
#include <string>
#include <map>

#include "../Environment/State.h"

TEST (StateAddDelta, ValidPositiveDelta)
{
	GameAgent::State state;
	float value = 12.0f;

	state.AddDelta("Delta 1", value);
	EXPECT_NO_THROW(state.GetDeltas().at("Delta 1"));
	EXPECT_EQ(state.GetDeltas().at("Delta 1"), value);
}

TEST(StateAddDelta, ValidNegativeDelta)
{
	GameAgent::State state;
	float value = -12.0f;

	state.AddDelta("Delta 1", value);
	EXPECT_NO_THROW(state.GetDeltas().at("Delta 1"));
	EXPECT_EQ(state.GetDeltas().at("Delta 1"), value);
}

TEST(StateAddDelta, ValidMultipleDeltas)
{
	GameAgent::State state;
	float value1 = 12.0f;
	float value2 = -12.0f;

	state.AddDelta("Delta 1", value1);
	state.AddDelta("Delta 2", value2);
	EXPECT_NO_THROW(state.GetDeltas().at("Delta 1"));
	EXPECT_EQ(state.GetDeltas().at("Delta 1"), value1);	
	EXPECT_NO_THROW(state.GetDeltas().at("Delta 2"));
	EXPECT_EQ(state.GetDeltas().at("Delta 2"), value2);
}

TEST(StateAddDelta, ValidNoDeltaName)
{
	GameAgent::State state;
	float value = 12.0f;

	state.AddDelta("", value);
	EXPECT_NO_THROW(state.GetDeltas().at(""));
	EXPECT_EQ(state.GetDeltas().at(""), value);
}

TEST(StateAddDelta, InvalidDupicateDelta)
{
	GameAgent::State state;
	float value = -12.0f;

	state.AddDelta("Delta 1", value);
	state.AddDelta("Delta 1", value);
	EXPECT_NO_THROW(state.GetDeltas().at("Delta 1"));
	EXPECT_EQ(state.Size(), 1);
}

TEST(StateRemoveDelta, ValidDeltaName)
{
	GameAgent::State state;
	float value = 12.0f;

	state.AddDelta("Delta 1", value);
	EXPECT_NO_THROW(state.GetDeltas().at("Delta 1"));
	EXPECT_EQ(state.GetDeltas().at("Delta 1"), value);

	state.RemoveDelta("Delta 1");
	EXPECT_EQ(state.Size(), 0);
}

TEST(StateRemoveDelta, InvalidDeltaNameDoesNotExist)
{
	GameAgent::State state;
	state.RemoveDelta("Delta 1");
	EXPECT_ANY_THROW(state.GetDeltas().at("Delta 1"));
	EXPECT_EQ(state.Size(), 0);
}

TEST(StateUpdateDelta, ValidPositiveDeltaName)
{
	GameAgent::State state;
	float value1 = 12.0f;
	float value2 = 24.0f;

	state.AddDelta("Delta 1", value1);
	EXPECT_EQ(state.GetDeltas().at("Delta 1"), value1);
	state.UpdateDelta("Delta 1", value2);
	EXPECT_EQ(state.GetDeltas().at("Delta 1"), value2);
}

TEST(StateUpdateDelta, ValidNegativeDeltaName)
{
	GameAgent::State state;
	float value1 = -12.0f;
	float value2 = -24.0f;

	state.AddDelta("Delta 1", value1);
	EXPECT_EQ(state.GetDeltas().at("Delta 1"), value1);
	state.UpdateDelta("Delta 1", value2);
	EXPECT_EQ(state.GetDeltas().at("Delta 1"), value2);
}

TEST(StateUpdateDelta, InvalidDeltaNameDoesNotExist)
{
	GameAgent::State state;
	float value = 12.0f;
	EXPECT_NO_THROW(state.UpdateDelta("Delta 1", value));
}

TEST(StateReset, ResetDeltas)
{
	GameAgent::State state;
	float value = 12.0f;

	state.AddDelta("Delta 1", value);
	state.AddDelta("Delta 2", value);
	state.Reset();
	EXPECT_EQ(state.Size(), 0);
}

TEST(StateSize, StateSize)
{
	GameAgent::State state;
	state.AddDelta("Delta 1", 12.0f);
	EXPECT_EQ(state.Size(), 1);
}

TEST(StateToVector, StateToVector)
{
	GameAgent::State state;
	float value1 = 12.0f;
	float value2 = 14.0f;

	state.AddDelta("Delta 1", value1);
	state.AddDelta("Delta 2", value2);
	auto stateVector = state.ToVector();
	int i = 0;
	for (auto delta : state.GetDeltas())
	{
		EXPECT_EQ(stateVector.at(i), delta.second);
		i++;
	}
}
