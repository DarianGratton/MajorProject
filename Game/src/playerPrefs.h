#pragma once

#include "CScript.h"

// TODO: Either refactor as a more robust solution or figure out another way to select the player weapons. Is a quick solution.
class PlayerPrefs 
{
public:
    static PlayerPrefs& Instance() 
    {
        static PlayerPrefs *instance = new PlayerPrefs();
        return *instance;
    }

    PlayerPrefs(PlayerPrefs const&) = delete;
    void operator=(PlayerPrefs const&) = delete;

    void SetWeapon1(int selection) { weapon1 = selection; }
    void SetWeapon2(int selection) { weapon2 = selection; }

    int GetWeapon1() { return weapon1; }
    int GetWeapon2() { return weapon2; }

private:
    PlayerPrefs() 
    {
        weapon1 = 0;
        weapon2 = 0;
    }

    int weapon1;
    int weapon2;
};