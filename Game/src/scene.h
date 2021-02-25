#pragma once

#include <string>

#include <entityx/entityx.h>

#include "Systems.h"

using namespace entityx;

class Scene {
public:
    Scene(std::string name, std::string filename) : name(name), filename(filename) {}

    bool load();
    inline std::string getName() { return name; }

private:
    std::string name;
    std::string filename;

    // TODO: Move to separate class as it doesn't fit the scene object
};