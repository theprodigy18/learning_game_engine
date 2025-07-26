#include "depch.h"
#include "Log.h"

#include <spdlog/sinks/stdout_color_sinks.h>

namespace Drop
{
	namespace
	{
        std::shared_ptr<Logger> g_coreLogger;
        std::shared_ptr<Logger> g_clientLogger;


	} // namespace anonymous

	void LogInit()
	{
        spdlog::set_pattern("%^[%T] %n: %v%$");
        g_coreLogger = spdlog::stdout_color_mt("ENGINE");
        g_coreLogger->set_level(spdlog::level::trace);
		g_clientLogger = spdlog::stdout_color_mt("CLIENT");
		g_clientLogger->set_level(spdlog::level::trace);
	}

	std::shared_ptr<Logger>& GetCoreLogger() { return g_coreLogger; }

	std::shared_ptr<Logger>& GetLogger() { return g_clientLogger; }

} // namespace Drop