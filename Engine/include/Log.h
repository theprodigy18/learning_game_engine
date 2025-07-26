#pragma once

#include "LogAPI.h"

namespace Drop
{
    void LogInit();

    std::shared_ptr<Logger>& GetCoreLogger();
} // namespace Drop

#ifndef DE_DIST

// Core log macros.
#define DE_CORE_TRACE(...) ::Drop::GetCoreLogger()->trace(__VA_ARGS__)
#define DE_CORE_INFO(...) ::Drop::GetCoreLogger()->info(__VA_ARGS__)
#define DE_CORE_WARN(...) ::Drop::GetCoreLogger()->warn(__VA_ARGS__)
#define DE_CORE_ERROR(...) ::Drop::GetCoreLogger()->error(__VA_ARGS__)

#else
#define DE_CORE_TRACE(...)
#define DE_CORE_INFO(...)
#define DE_CORE_WARN(...)
#define DE_CORE_ERROR(...)
#endif // !DO_DIST

#ifndef DE_NO_ASSERT
#define DE_CORE_ASSERT(x, ...)          \
    {                                   \
        if (!(x))                       \
        {                               \
            DE_CORE_ERROR(__VA_ARGS__); \
            __debugbreak();             \
        }                               \
    }
#else
#define DE_CORE_ASSERT(x, ...)
#endif // !DE_NO_ASSERT