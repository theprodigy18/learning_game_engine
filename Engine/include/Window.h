#pragma once

#ifdef DE_PLATFORM_WINDOWS
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>

using WindowHandle  = HWND;
using WindowContext = HDC;
#endif

namespace Drop
{
    enum WINDOW_TYPE
    {
        WINDOW_TYPE_FRAME   = BIT(0),
        WINDOW_TYPE_RESIZE = BIT(1),
		WINDOW_TYPE_MINIMIZE = BIT(2),
		WINDOW_TYPE_MAXIMIZE = BIT(3),
		WINDOW_TYPE_FULLSCREEN = BIT(4),


        WINDOW_TYPE_POPUP     = BIT(2),
        WINDOW_TYPE_CHILD     = BIT(4) | BIT(0), // Contain no frame.
    };

    struct DE_API WindowInfo
    {
        WindowHandle  handle {nullptr};
        WindowContext context {nullptr};
    };
} // namespace Drop