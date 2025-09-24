#include "../include/logger.hpp"

namespace Core {
std::shared_ptr<spdlog::logger> Logger::s_instance = nullptr;
} // namespace Core
