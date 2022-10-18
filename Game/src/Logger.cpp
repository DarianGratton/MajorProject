#include "Logger.h"

#include <spdlog/sinks/stdout_color_sinks.h>

void Logger::Init() 
{
    spdlog::set_pattern("%^[%T] %n: %v%$");
    
    logger = spdlog::stdout_color_mt("Log");
    logger->set_level(spdlog::level::trace);
}