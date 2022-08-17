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
	storage.Clear();
	storage.Save(state, 50.0f);

	auto cachedStates = storage.Load();
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
	storage.Clear();
	storage.Save(state, 50.0f);
	storage.Save(state2, 75.0f);

	auto cachedStates = storage.Load();
	GameAgent::State cachedState = cachedStates.at(0).first;
	EXPECT_EQ(state, cachedState);
	EXPECT_EQ(cachedStates.at(0).second, 50.0f);
	
    cachedState = cachedStates.at(1).first;
	EXPECT_EQ(state2, cachedState);
	EXPECT_EQ(cachedStates.at(1).second, 75.0f);
}

TEST(StorageSave, StateAlreadyInFile)
{
	GameAgent::State state;
	state.AddDelta("Delta 1", 13.0f);
	state.AddDelta("Delta 2", 12.0f);

	UtilityStorage storage;
	storage.Clear();
	storage.Save(state, 50.0f);
	storage.Save(state, 100.0f);

	auto cachedStates = storage.Load();
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
	storage.Clear();
	storage.Save(state, 50.0f);

	UtilityStorage storage2;
	auto cachedStates = storage2.Load();
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
	storage.Clear();
	storage.Save(state, 50.0f);

	auto cachedStates = storage.Load();
	EXPECT_EQ(cachedStates.size(), 1);

	storage.Clear();
	cachedStates = storage.Load();
	EXPECT_EQ(cachedStates.size(), 0);
}

TEST(StorageSearch, ValidState)
{
	GameAgent::State state;
	state.AddDelta("Delta 1", 13.0f);
	state.AddDelta("Delta 2", 12.0f);

	GameAgent::State searchState1;
	searchState1.AddDelta("Delta 1", 13.0f);
	
	GameAgent::State searchState2;
	searchState2.AddDelta("Delta 1", 15.0f);

	UtilityStorage storage;
	storage.Clear();
	storage.Save(state, 50.0f);

	auto searchedStates = storage.Search(searchState1);
	EXPECT_EQ(searchedStates.size(), 1);
	EXPECT_EQ(state, searchedStates.at(0).first);

	storage.Clear();
	searchedStates = storage.Search(searchState2);
	EXPECT_EQ(searchedStates.size(), 0);
}
