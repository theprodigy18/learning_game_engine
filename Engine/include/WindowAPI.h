#pragma once

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
    struct WindowInfo;

    DE_API struct WindowRect
    {
        i32 left {0};
        i32 top {0};
        i32 right {0};
        i32 bottom {0};
    };

    DE_API struct WindowSize
    {
        WindowRect clientRect;
        WindowRect fullRect;
    };

	DE_API struct WindowPoint
	{
		union
		{
			struct
			{
				i32 x;
				i32 y;
			};
			struct
			{
				i32 w;
				i32 h;
			};
		};
	};

	using ResizeCallback = bool (*)(const WindowSize* pSize);
	using CloseCallback  = bool (*)();
	using SetMinMaxCallback = bool (*)(WindowPoint& outMin, WindowPoint& outMax);

    DE_API struct WindowEventCallback
    {
        ResizeCallback OnResize {nullptr};
        CloseCallback  OnClose {nullptr};
		SetMinMaxCallback OnSetMinMax {nullptr};
    };

    DE_API struct WindowInitProps
    {
        const char*         title;
        u32                 width {0};
        u32                 height {0};
        u32                 flags {WINDOW_TYPE_NORMAL};
        WindowEventCallback events;
    };

    DE_API bool PlatformCreateWindow(WindowInfo** ppInfo, const WindowInitProps* pProps, const WindowInfo* pParentInfo = nullptr);
    DE_API void PlatformDestroyWindow(WindowInfo** ppInfo);

    DE_API bool PlatformPollEvents(); // false if all windows are closed.

    DE_API void PlatformResizeWindow(const WindowInfo* pInfo, const WindowSize* pSize);

	DE_API void PlatformSendQuit(i32 exitCode = 0);
} // namespace Drop
