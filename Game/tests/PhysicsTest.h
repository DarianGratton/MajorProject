#include <gtest/gtest.h>
#include <box2d/box2d.h>

#include "src/Physics/PhysicsManager.h"
#include "src/Physics/ContactListener.h"

using namespace std;

TEST (BeginContact, NoUserDataA)
{
    b2Contact test = new b2Contact();
}