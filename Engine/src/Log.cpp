#include "depch.h"
#include "Log.h"

#include <spdlog/sinks/stdout_color_sinks.h>

namespace Drop
{
	namespace
	{
        std::shared_ptr<spdlog::logger> g_coreLogger;
	} // namespace anonymous

	void LogInit()
	{
        spdlog::set_pattern("%^[%T] %n: %v%$");
        g_coreLogger = spdlog::stdout_color_mt("ENGINE");
        g_coreLogger->set_level(spdlog::level::trace);
	}

	std::shared_ptr<spdlog::logger>& GetCoreLogger() { return g_coreLogger; }
} // namespace Drop