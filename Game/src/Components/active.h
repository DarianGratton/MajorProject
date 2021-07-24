#pragma once

#include "entityx/entityx.h"

#include "../logger.h"

struct Active {
    Active(bool active) : isActive(active) {}

    bool isActive;
};