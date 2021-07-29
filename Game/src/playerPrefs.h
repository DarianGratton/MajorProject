#pragma once

#include "cScript.h"

// TODO: Either refactor as a more robust solution or figure out another way to select the player weapons. Is a quick solution.
class PlayerPrefs {
public:
    static PlayerPrefs& instance() {
        static PlayerPrefs *instance = new PlayerPrefs();
        return *instance;
    }

    PlayerPrefs(PlayerPrefs const&) = delete;
    void operator=(PlayerPrefs const&) = delete;

    void setWeapon1(int selection) { weapon1 = selection; }
    void setWeapon2(int selection) { weapon2 = selection; }

    int getWeapon1() { return weapon1; }
    int getWeapon2() { return weapon2; }

private:
    PlayerPrefs() {
        weapon1 = 0;
        weapon2 = 0;
    }

    int weapon1;
    int weapon2;
};