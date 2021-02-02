#include "spdlog/spdlog.h"

class Logger {
public:
    static void init();

    inline static std::shared_ptr<spdlog::logger>& log() { return logger; }
    
private:
    static std::shared_ptr<spdlog::logger> logger;
};

// Logging macros
#define LOG_TRACE(...)  Logger::log()->trace(__VA_ARGS__)
#define LOG_INFO(...)   Logger::log()->info(__VA_ARGS__)
#define LOG_WARN(...)   Logger::log()->warn(__VA_ARGS__)
#define LOG_ERROR(...)  Logger::log()->error(__VA_ARGS__)
