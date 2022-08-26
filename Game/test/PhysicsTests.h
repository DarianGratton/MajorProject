#pragma once

#include <gtest/gtest.h>

#include <box2d/box2d.h>

#include "../src/Physics/PhysicsManager.h"

using namespace std;

TEST (CreateWorld, ValidXYValues)
{
    PhysicsManager::Instance().CreateWorld(0.0f, 0.0f);
    EXPECT_NE(PhysicsManager::Instance().GetWorld(), nullptr);
}

TEST (CreateWorld, NegativeXYValues)
{
    PhysicsManager::Instance().CreateWorld(-1.0f, -1.0f);
    EXPECT_NE(PhysicsManager::Instance().GetWorld(), nullptr);
}

TEST (IntToCategory, ValidInt)
{
    PhysicsManager::bodyCategory bodyCat = PhysicsManager::Instance().IntToCategory(1);
    EXPECT_EQ(bodyCat, PhysicsManager::Instance().BOUNDARY);
}

TEST (IntToCategory, ValidInt2)
{
    PhysicsManager::bodyCategory bodyCat = PhysicsManager::Instance().IntToCategory(3);
    EXPECT_EQ(bodyCat, PhysicsManager::Instance().ENEMY);
}

TEST (IntToCategory, OutOfRange)
{
    PhysicsManager::bodyCategory bodyCat = PhysicsManager::Instance().IntToCategory(232);
    EXPECT_EQ(bodyCat, PhysicsManager::Instance().BOUNDARY);
}


