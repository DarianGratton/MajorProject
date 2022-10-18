#pragma once

#include <vector>
#include <algorithm>
#include <random>
#include <iterator>
#include <iostream>

#include "../Logger.h"

/*
  TrainingData

  Defines the training data used by the agent (AI) during training.

  List of Data: 

  Player Actions:
  0 = Nothing
  1 = Up
  2 = Down
  3 = Right
  4 = Left
  5 = Weapon 1
  6 = Weapon 2

  Player Moves:
  NotMoving
  MovingHorizontally
  MovingVertically
  MovingDiagonallyTopRightToBottomLeft
  MovingDiagonallyTopLeftToBottomRight
  MovingDiagonallyTopLeft
  MovingDiagonallyTopRight

  Weapons:
  1 = Sword
  2 = Shield
  3 = Bow
  4 = Gun
  5 = Grenade/Bomb

  Author: Darian G.
*/
struct TrainingData
{

	/*
	  Gets a random weapon combination.
	*/
	static const std::pair<float, float> GetRandWeaponCombination()
	{
		std::vector<std::vector<unsigned int>> pickedWeapons;
		std::sample(
			TrainingData::weaponComps.begin(),
			TrainingData::weaponComps.end(),
			std::back_inserter(pickedWeapons),
			1,
			std::mt19937{ std::random_device{}() }
		);

		return std::make_pair(pickedWeapons[0][0], pickedWeapons[0][1]);
	}

	/*
	  Common moves that a character can make.
	*/
	enum class CommonMoves
	{
		NotMoving,
		MovingHorizontally,
		MovingVertically,
		MovingDiagonallyTopRightToBottomLeft,
		MovingDiagonallyTopLeftToBottomRight,
		MovingDiagonallyTopLeft,
		MovingDiagonallyTopRight
	};

	/*
	  Get a list of repeatable player actions for the common move specified.
	  params:
			- commonMove: Common move to return.
			- attacking: Is player attacking during the common move set.
	  returns:
			- A list of moves.
	*/
	static const std::vector<unsigned int> GetCommonMove(CommonMoves commonMove, bool isAttacking)
	{
		switch (commonMove)
		{
		case CommonMoves::NotMoving:
			return GetCommonNotMoving(isAttacking);
			break;
		case CommonMoves::MovingHorizontally:
			return GetCommonMoveMoving({ 3 }, { 4 }, isAttacking, 30);
			break;
		case CommonMoves::MovingVertically:
			return GetCommonMoveMoving({ 1 }, { 2 }, isAttacking, 30);
			break;
		case CommonMoves::MovingDiagonallyTopRightToBottomLeft:
			return GetCommonMoveMoving({ 1, 3 }, { 2, 4 }, isAttacking, 30);
			break;
		case CommonMoves::MovingDiagonallyTopLeftToBottomRight:
			return GetCommonMoveMoving({ 1, 4 }, { 2, 3 }, isAttacking, 30);
			break;
		case CommonMoves::MovingDiagonallyTopLeft:
			return GetCommonMoveMoving({ 1, 4 }, { 1, 4 }, isAttacking, 30);
			break;
		case CommonMoves::MovingDiagonallyTopRight:
			return GetCommonMoveMoving({ 1, 3 }, { 1, 3 }, isAttacking, 30);
			break;
		default:
			LOG_ERROR("Common move choosen not supported");
			return { };
		}
	}

	/* 
	  Get all possible weapon combinations.
	  returns:
			- A vector containing all the weapon combinations.
	*/
	static const std::vector<std::vector<unsigned int>> GetWeaponComps() { return weaponComps; }

private:

	/*
	  Weapon Combinations.
	*/
	static const std::vector<std::vector<unsigned int>> weaponComps;

	/*
	  Get a list of repeatable player actions for the common move Player not
	  moving.
	  params:
			- attacking: Is player attacking during the common move set.
	  returns:
			- A list of moves.
	*/
	static const std::vector<unsigned int> GetCommonNotMoving(bool attacking)
	{
		if (attacking)
			return { 0, 5, 6 };

		return { 0 };
	}

	/*
	  Get a list of repeatable player actions for the common move Player
	  moving in two directions.
	  params:
			- attacking: Is player attacking during the common move set.
	  returns:
			- A list of moves.
	*/
	static const std::vector<unsigned int> GetCommonMoveMoving(std::vector<unsigned int> direction1, std::vector<unsigned int> direction2, bool attacking, int actionLength)
	{
		std::vector<unsigned int> moveset;

		// Direction 1
		for (int i = 0; i < actionLength; i++)
		{
			// Concatenate direction 1 to moveset
			moveset.insert(moveset.end(), direction1.begin(), direction1.end());

			if (attacking)
			{
				// 30% Chance of Attacking
				int randomNumber = (rand() % 10) + 1;
				if (randomNumber <= 3)
				{
					moveset.push_back(5);
					moveset.push_back(6);
				}
			}
		}

		// Direction 2
		for (int i = 0; i < actionLength; i++)
		{
			// Concatenate direction 2 to moveset
			moveset.insert(moveset.end(), direction2.begin(), direction2.end());

			if (attacking)
			{
				// 30% Chance of Attacking
				int randomNumber = (rand() % 10) + 1;
				if (randomNumber <= 3)
				{
					moveset.push_back(5);
					moveset.push_back(6);
				}
			}
		}

		return moveset;
	}

};
