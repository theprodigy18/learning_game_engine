#include "depch.h"
#include "Window.h"

namespace Drop
{
    namespace
    {
        HINSTANCE g_hInstance {nullptr};

        LRESULT CALLBACK InternalWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
        {
            return DefWindowProcW(hwnd, msg, wParam, lParam);
        }

        const wchar_t* FromCharToWChar(const char* string)
        {
            u64      titleLength     = strlen(string);
            i32      wideTitleLength = MultiByteToWideChar(CP_UTF8, 0, string, (i32)titleLength, nullptr, 0);
            wchar_t* wideTitle       = new wchar_t[wideTitleLength + 1];
            MultiByteToWideChar(CP_UTF8, 0, string, titleLength, wideTitle, wideTitleLength);
            wideTitle[wideTitleLength] = L'\0';
            return wideTitle;
        }

    } // anonymous namespace

    bool PlatformInit()
    {
        g_hInstance = GetModuleHandleW(nullptr);
        DE_CORE_ASSERT(g_hInstance, "Failed to get module handle!");

        WNDCLASSEXW wcex {};
        wcex.cbSize        = sizeof(WNDCLASSEX);
        wcex.style         = CS_OWNDC;
        wcex.hInstance     = g_hInstance;
        wcex.cbClsExtra    = 0;
        wcex.cbWndExtra    = 0;
        wcex.lpfnWndProc   = InternalWndProc;
        wcex.hIcon         = LoadIcon(nullptr, IDI_APPLICATION);
        wcex.hIconSm       = LoadIcon(nullptr, IDI_APPLICATION);
        wcex.hCursor       = LoadCursor(nullptr, IDC_ARROW);
        wcex.lpszClassName = L"DropWindowClass";
        wcex.lpszMenuName  = nullptr;

        if (!RegisterClassExW(&wcex))
        {
            DE_CORE_ASSERT(false, "Failed to register window class!");
            return false;
        }

        return true;
    }

    WindowInfo PlatformCreateWindow(const char* title, u32 width, u32 height, u32 flags, WindowHandle parent)
    {
        DE_CORE_ASSERT(g_hInstance, "Failed to get module handle!");

        WindowInfo info {};

        DWORD dwStyle {WS_OVERLAPPEDWINDOW};

        HWND hwnd {CreateWindowExW(
            0,
            L"DropWindowClass", FromCharToWChar(title),
            dwStyle,
            CW_USEDEFAULT, CW_USEDEFAULT,
            width, height,
            parent, nullptr, g_hInstance, nullptr)};

        if (!hwnd)
        {
            DE_CORE_ASSERT(false, "Failed to create window!");
            return info;
        }

        HDC dc {GetDC(hwnd)};
        DE_CORE_ASSERT(dc, "Failed to get device context!");


        info.handle  = hwnd;
        info.context = dc;

		ShowWindow(hwnd, SW_SHOW);
		UpdateWindow(hwnd);

		DE_CORE_TRACE("Window created: {0}x{1}", width, height);

        return info;
    }

    void PlatformDestroyWindow(WindowInfo& info)
    {
        DE_CORE_ASSERT(info.handle, "Failed to get window handle!");
        DE_CORE_ASSERT(info.context, "Failed to get device context!");

        ReleaseDC(info.handle, info.context);
        DestroyWindow(info.handle);
    }

	void PlatformShutdown()
	{
		UnregisterClassW(L"DropWindowClass", g_hInstance);
	}

} // namespace Drop