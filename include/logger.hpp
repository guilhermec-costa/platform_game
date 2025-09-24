#pragma once

#include <memory>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace Core {

class Logger {
public:
    static void init() {
        if (!s_instance) {
            auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
            console_sink->set_level(spdlog::level::trace);
            console_sink->set_pattern("[%T] [%^%l%$] %v");

            s_instance = std::make_shared<spdlog::logger>("game_logger", console_sink);
            s_instance->set_level(spdlog::level::trace);
            spdlog::set_default_logger(s_instance);
        }
    }

    static std::shared_ptr<spdlog::logger>& get() {
        return s_instance;
    }

private:
    static std::shared_ptr<spdlog::logger> s_instance;
};

} // namespace Core

#define LOG_TRACE(...) Core::Logger::get()->trace(__VA_ARGS__)
#define LOG_INFO(...)  Core::Logger::get()->info(__VA_ARGS__)
#define LOG_DEBUG(...)  Core::Logger::get()->debug(__VA_ARGS__)
#define LOG_WARN(...)  Core::Logger::get()->warn(__VA_ARGS__)
#define LOG_ERROR(...) Core::Logger::get()->error(__VA_ARGS__)
#define LOG_CRITICAL(...) Core::Logger::get()->critical(__VA_ARGS__)
