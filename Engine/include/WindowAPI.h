#pragma once

#ifdef DE_PLATFORM_WINDOWS
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>

using WindowHandle  = HWND;
using WindowContext = HDC;
#endif

#define WINDOW_TYPE_FRAME BIT(0)
#define WINDOW_TYPE_RESIZE BIT(1)
#define WINDOW_TYPE_MINIMIZE BIT(2)
#define WINDOW_TYPE_MAXIMIZE BIT(3)
#define WINDOW_TYPE_FULLSCREEN BIT(4)

#define WINDOW_TYPE_CHILD BIT(5) // You can only pair this with resize and nothing else.
#define WINDOW_TYPE_POPUP BIT(6) | WINDOW_TYPE_FRAME
#define WINDOW_TYPE_NORMAL WINDOW_TYPE_FRAME | WINDOW_TYPE_RESIZE | WINDOW_TYPE_MINIMIZE | WINDOW_TYPE_MAXIMIZE

namespace Drop
{
    DE_API struct WindowInfo
    {
        WindowHandle  handle {nullptr};
        WindowContext context {nullptr};
    };

    DE_API WindowInfo PlatformCreateWindow(const char* title, u32 width, u32 height, u32 flags, WindowHandle parent = nullptr);
    DE_API void       PlatformDestroyWindow(WindowInfo& info);
} // namespace Drop

