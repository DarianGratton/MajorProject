#pragma once

#include <string>

#include "../logger.h"

struct Name {
    Name(std::string name) : name(name) {}

    std::string name;
};
