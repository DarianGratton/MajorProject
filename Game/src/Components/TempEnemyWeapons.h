#pragma once

// TODO: Hacky way of testing diffent weapons on the enemy. Will be deleted once the agent has been implemented.
struct TempEnemyWeapons
{
    TempEnemyWeapons(unsigned int weapon1, unsigned int weapon2) 
        : weapon1(weapon1), weapon2(weapon2) {}

    unsigned int weapon1;
    unsigned int weapon2;
};