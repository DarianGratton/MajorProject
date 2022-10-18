#pragma once

#include <entityx/entityx.h>

#include <string>

using namespace entityx;

class Scene 
{
public:
    Scene(std::string name, std::string filename) : name(name), filename(filename) {}

    bool Load();
    inline std::string GetName() { return name; }

private:
    std::string name;
    std::string filename;
};