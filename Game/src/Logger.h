#pragma once

#include "spdlog/spdlog.h"

class Logger 
{
public:
    static void Init();

    inline static std::shared_ptr<spdlog::logger>& Log() { return logger; }
    
private:
    static std::shared_ptr<spdlog::logger> logger;
};

// Logging macros
#define LOG_TRACE(...)  Logger::Log()->trace(__VA_ARGS__)
#define LOG_INFO(...)   Logger::Log()->info(__VA_ARGS__)
#define LOG_WARN(...)   Logger::Log()->warn(__VA_ARGS__)
#define LOG_ERROR(...)  Logger::Log()->error(__VA_ARGS__)
