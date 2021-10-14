#pragma once

#include "../Logger.h"

struct Active 
{
    Active(bool active) : isActive(active) {}

    bool isActive;
};