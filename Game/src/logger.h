#include "spdlog/spdlog.h"

class Logger {
public:
    static Logger& instance() {
        static Logger *instance = new Logger();
        return *instance;
    }

    
private:
    static std::shared_ptr<spdlog::logger> logger;

    void init();
};

// Logging macros
#define LOG_TRACE(...)  ::Logger::instance()->trace(__VA_ARGS__)
#define LOG_INFO(...)   ::Logger::instance()->info(__VA_ARGS__)
#define LOG_WARN(...)   ::Logger::instance()->warn(__VA_ARGS__)
#define LOG_ERROR(...)  ::Logger::instance()->error(__VA_ARGS__)
#define LOG_FATAL(...)  ::Logger::instance()->fatal(__VA_ARGS__)
