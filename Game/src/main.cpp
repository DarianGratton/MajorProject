#include <iostream>

#include "Logger.h"

int main(int argc, char** argv) {
    Logger::init();
    LOG_INFO("Logger Initialized");

    return 0;
}