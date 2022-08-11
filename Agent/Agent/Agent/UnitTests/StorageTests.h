#include <gtest/gtest.h>

#include <vector>
#include <string>
#include <map>

#include "../Storage/UtilityStorage.h"

TEST(StorageSave, SaveToNewFile)
{
	GameAgent::State state;
	state.AddDelta("Delta 1", 13.0f);
	state.AddDelta("Delta 2", 12.0f);

	UtilityStorage storage;
	storage.Save(state, 50.0f);

	auto cachedStates = storage.GetCachedStates();
	GameAgent::State cachedState = cachedStates.at(0).first;
	EXPECT_EQ(state, cachedState);
	EXPECT_EQ(cachedStates.at(0).second, 50.0f);
} 

TEST(StorageSave, SaveToExistingFile)
{
	GameAgent::State state;
	state.AddDelta("Delta 1", 13.0f);
	state.AddDelta("Delta 2", 12.0f);
	
	GameAgent::State state2;
	state.AddDelta("Delta 12", 112.0f);
	state.AddDelta("Delta 11", 14.0f);

	UtilityStorage storage;
	storage.Save(state, 50.0f)
	storage.Save(state2, 75.0f);

	auto cachedStates = storage.GetCachedStates();
	GameAgent::State cachedState = cachedStates.at(0).first;
	EXPECT_EQ(state, cachedState);
	EXPECT_EQ(cachedStates.at(0).second, 50.0f);
	
	GameAgent::State cachedState = cachedStates.at(1).first;
	EXPECT_EQ(state, cachedState);
	EXPECT_EQ(cachedStates.at(0).second, 75.0f);
}

TEST(StorageSave, StateAlreadyInFile)
{
	GameAgent::State state;
	state.AddDelta("Delta 1", 13.0f);
	state.AddDelta("Delta 2", 12.0f);

	UtilityStorage storage;
	storage.Save(state, 50.0f);
	storage.Save(state, 75.0f);

	auto cachedStates = storage.GetCachedStates();
	GameAgent::State cachedState = cachedStates.at(0).first;
	EXPECT_EQ(state, cachedState);
	EXPECT_EQ(cachedStates.at(0).second, 75.0f);
}

TEST(StorageLoad, LoadFromFile)
{
	GameAgent::State state;
	state.AddDelta("Delta 1", 13.0f);
	state.AddDelta("Delta 2", 12.0f);

	UtilityStorage storage;
	storage.Save(state, 50.0f);

	UtilityStorage storage2;
	storage.Load();

	auto cachedStates = storage2.GetCachedStates();
	GameAgent::State cachedState = cachedStates.at(0).first;
	EXPECT_EQ(state, cachedState);
	EXPECT_EQ(cachedStates.at(0).second, 50.0f);
}

TEST(StorageClear, ClearStorage)
{
	GameAgent::State state;
	state.AddDelta("Delta 1", 13.0f);
	state.AddDelta("Delta 2", 12.0f);

	UtilityStorage storage;
	storage.Save(state, 50.0f);

	auto cachedStates = storage.GetCachedStates();
	EXPECT_EQ(cachedStates.size(), 1);

	storage.Clear();
	auto cachedStates = storage.GetCachedStates();
	EXPECT_EQ(cachedStates.size(), 0);
}
