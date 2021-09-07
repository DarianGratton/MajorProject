#pragma once

#include <entityx/entityx.h>

#include <string>

#include "Systems.h"

using namespace std;
using namespace entityx;

class Scene 
{
public:
    Scene(string name, std::string filename) : name(name), filename(filename) {}

    bool Load();
    inline string GetName() { return name; }

private:
    string name;
    string filename;
};