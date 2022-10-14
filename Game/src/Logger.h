#pragma once

#include "spdlog/spdlog.h"

class Logger 
{
public:
    static Logger& Instance()
    {
        static Logger* instance = new Logger();
        return *instance;
    }

    Logger(Logger const&) = delete;
    void operator=(Logger const&) = delete;

    void Init();
    // static void Init();

    inline std::shared_ptr<spdlog::logger>& Log() { return logger; }
    
private:
    Logger() {}

    std::shared_ptr<spdlog::logger> logger;
};

// Logging macros
#define LOG_TRACE(...)  Logger::Instance().Log()->trace(__VA_ARGS__)
#define LOG_INFO(...)   Logger::Instance().Log()->info(__VA_ARGS__)
#define LOG_WARN(...)   Logger::Instance().Log()->warn(__VA_ARGS__)
#define LOG_ERROR(...)  Logger::Instance().Log()->error(__VA_ARGS__) 
