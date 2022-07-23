#include <gtest/gtest.h>

#include <vector>
#include <string>
#include <map>

#include "../Environment/State.h"

using namespace std;

TEST (AddDelta, ValidPositiveDelta)
{
	State state;
	float value = 12.0f;

	state.AddDelta("Delta 1", value);
	EXPECT_NO_THROW(state.GetDeltas().at("Delta 1"));
	EXPECT_EQ(state.GetDeltas().at("Delta 1"), value);
}

TEST(AddDelta, ValidNegativeDelta)
{
	State state;
	float value = -12.0f;

	state.AddDelta("Delta 1", value);
	EXPECT_NO_THROW(state.GetDeltas().at("Delta 1"));
	EXPECT_EQ(state.GetDeltas().at("Delta 1"), value);
}

TEST(AddDelta, ValidMultipleDeltas)
{
	State state;
	float value1 = 12.0f;
	float value2 = -12.0f;

	state.AddDelta("Delta 1", value1);
	state.AddDelta("Delta 2", value2);
	EXPECT_NO_THROW(state.GetDeltas().at("Delta 1"));
	EXPECT_EQ(state.GetDeltas().at("Delta 1"), value1);	
	EXPECT_NO_THROW(state.GetDeltas().at("Delta 2"));
	EXPECT_EQ(state.GetDeltas().at("Delta 2"), value2);
}

TEST(AddDelta, ValidNoDeltaName)
{
	State state;
	float value = 12.0f;

	state.AddDelta("", value);
	EXPECT_NO_THROW(state.GetDeltas().at(""));
	EXPECT_EQ(state.GetDeltas().at(""), value);
}

TEST(AddDelta, InvalidDupicateDelta)
{
	State state;
	float value = -12.0f;

	state.AddDelta("Delta 1", value);
	state.AddDelta("Delta 1", value);
	EXPECT_NO_THROW(state.GetDeltas().at("Delta 1"));
	EXPECT_EQ(state.Size(), 1);
}

TEST(RemoveDelta, ValidDeltaName)
{
	State state;
	float value = 12.0f;

	state.AddDelta("Delta 1", value);
	EXPECT_NO_THROW(state.GetDeltas().at("Delta 1"));
	EXPECT_EQ(state.GetDeltas().at("Delta 1"), value);

	state.RemoveDelta("Delta 1");
	EXPECT_EQ(state.Size(), 0);
}

TEST(RemoveDelta, InvalidDeltaNameDoesNotExist)
{
	State state;
	state.RemoveDelta("Delta 1");
	EXPECT_ANY_THROW(state.GetDeltas().at("Delta 1"));
	EXPECT_EQ(state.Size(), 0);
}

TEST(UpdateDelta, ValidPositiveDeltaName)
{
	State state;
	float value1 = 12.0f;
	float value2 = 24.0f;

	state.AddDelta("Delta 1", value1);
	EXPECT_EQ(state.GetDeltas().at("Delta 1"), value1);
	state.UpdateDelta("Delta 1", value2);
	EXPECT_EQ(state.GetDeltas().at("Delta 1"), value2);
}

TEST(UpdateDelta, ValidNegativeDeltaName)
{
	State state;
	float value1 = -12.0f;
	float value2 = -24.0f;

	state.AddDelta("Delta 1", value1);
	EXPECT_EQ(state.GetDeltas().at("Delta 1"), value1);
	state.UpdateDelta("Delta 1", value2);
	EXPECT_EQ(state.GetDeltas().at("Delta 1"), value2);
}

TEST(UpdateDelta, InvalidDeltaNameDoesNotExist)
{
	State state;
	float value = 12.0f;
	EXPECT_NO_THROW(state.UpdateDelta("Delta 1", value));
}

TEST(Reset, ResetDeltas)
{
	State state;
	float value = 12.0f;

	state.AddDelta("Delta 1", value);
	state.AddDelta("Delta 2", value);
	state.Reset();
	EXPECT_EQ(state.Size(), 0);
}

TEST(Size, StateSize)
{
	State state;
	state.AddDelta("Delta 1", 12.0f);
	EXPECT_EQ(state.Size(), 1);
}

TEST(ToVector, StateToVector)
{
	State state;
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
