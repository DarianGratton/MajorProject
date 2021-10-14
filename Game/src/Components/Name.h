#pragma once

#include <string>

#include "../Logger.h"

struct Name 
{
    Name(std::string name) : name(name) {}

    std::string name;
};
