#pragma once

#include <string>

#include <entityx/entityx.h>

#include "Systems.h"

using namespace entityx;

class Scene {
public:
    Scene(std::string filename);

    void load(std::string filename);
    inline std::string getName() { return name; }

private:
    std::string name;
    std::string filename;
};