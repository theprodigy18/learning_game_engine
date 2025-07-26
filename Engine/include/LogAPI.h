#pragma once

#include <spdlog/spdlog.h>


namespace Drop
{
	using Logger = spdlog::logger;

    DE_API std::shared_ptr<Logger>& GetLogger();
} // namespace Drop

#ifndef DE_DIST 
#define DE_TRACE(...) ::Drop::GetLogger()->trace(__VA_ARGS__)
#define DE_INFO(...) ::Drop::GetLogger()->info(__VA_ARGS__)
#define DE_WARN(...) ::Drop::GetLogger()->warn(__VA_ARGS__)
#define DE_ERROR(...) ::Drop::GetLogger()->error(__VA_ARGS__)
#else 
#define DE_TRACE(...)
#define DE_INFO(...)
#define DE_WARN(...)
#define DE_ERROR(...)
#endif // !DE_DIST