#include "depch.h"

#ifdef DE_PLATFORM_WINDOWS
#include "Window.h"

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>

namespace Drop
{
    struct WindowInfo
    {
        HWND handle {nullptr};
        HDC  context {nullptr};
    };

    namespace
    {
        HINSTANCE g_hInstance {nullptr};

        LRESULT CALLBACK InternalWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
        {
            bool handled {false};

            switch (msg)
            {
            case WM_SIZE:
            {
                auto* pEvents {reinterpret_cast<WindowEventCallback*>(GetWindowLongPtrW(hwnd, GWLP_USERDATA))};
                if (pEvents && pEvents->OnResize)
                {
                    WindowSize* pSize {new WindowSize {}};

                    POINT pt {};
                    RECT  rc {};
                    ClientToScreen(hwnd, &pt);
                    GetClientRect(hwnd, &rc);
                    pSize->clientRect = {pt.x, pt.y, rc.right + pt.x, rc.bottom + pt.y};
                    GetWindowRect(hwnd, &rc);
                    pSize->fullRect = {rc.left, rc.top, rc.right, rc.bottom};

                    if (pEvents->OnResize(pSize))
                    {
                        handled = true;
                    }
                    delete pSize;
                }
                break;
            }
            case WM_GETMINMAXINFO:
			{
				auto* pEvents {reinterpret_cast<WindowEventCallback*>(GetWindowLongPtrW(hwnd, GWLP_USERDATA))};
				if (pEvents && pEvents->OnSetMinMax)
				{
					MINMAXINFO* pMinMaxInfo {reinterpret_cast<MINMAXINFO*>(lParam)};
                    WindowPoint min {}, max {};
					if (pEvents->OnSetMinMax(min, max))
					{
						pMinMaxInfo->ptMinTrackSize.x = min.x;
						pMinMaxInfo->ptMinTrackSize.y = min.y;

						pMinMaxInfo->ptMaxTrackSize.x = max.x;
						pMinMaxInfo->ptMaxTrackSize.y = max.y;

						handled = true;
					}
				}
				break;
			}
            case WM_CLOSE:
            {
                auto* pEvents {reinterpret_cast<WindowEventCallback*>(GetWindowLongPtrW(hwnd, GWLP_USERDATA))};
                if (pEvents && pEvents->OnClose)
                {
                    if (pEvents->OnClose())
                    {
                        handled = true;
                    }
                }
                break;
            }
            default:
                break;
            }

            return handled ? 0 : DefWindowProcW(hwnd, msg, wParam, lParam);
        }

        const wchar_t* FromCharToWChar(const char* string)
        {
            u64 titleLength {strlen(string)};
            i32 wideTitleLength {MultiByteToWideChar(CP_UTF8, 0, string, (i32) titleLength, nullptr, 0)};
            wchar_t* wideTitle {new wchar_t[wideTitleLength + 1]};
            MultiByteToWideChar(CP_UTF8, 0, string, titleLength, wideTitle, wideTitleLength);
            wideTitle[wideTitleLength] = L'\0';
            return wideTitle;
        }

    } // anonymous namespace

    bool PlatformInit()
    {
        SetProcessDPIAware();

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

    bool PlatformCreateWindow(WindowInfo** ppInfo, const WindowInitProps* pProps, const WindowInfo* pParentInfo)
    {
        DE_CORE_ASSERT(g_hInstance, "Failed to get module handle!");
		*ppInfo = nullptr;

        DWORD dwStyle {WS_OVERLAPPEDWINDOW};

        HWND hwnd {CreateWindowExW(
            0,
            L"DropWindowClass", FromCharToWChar(pProps->title),
            dwStyle,
            CW_USEDEFAULT, CW_USEDEFAULT,
            pProps->width, pProps->height,
            pParentInfo ? pParentInfo->handle : nullptr, nullptr, g_hInstance, nullptr)};

        if (!hwnd)
        {
            DE_CORE_ASSERT(false, "Failed to create window!");
            return false;
        }

        HDC dc {GetDC(hwnd)};
        DE_CORE_ASSERT(dc, "Failed to get device context!");

        WindowInfo* pInfo {new WindowInfo {}};
        pInfo->handle  = hwnd;
        pInfo->context = dc;

        WindowEventCallback* events {new WindowEventCallback {}};
        *events = pProps->events;

        SetWindowLongPtrW(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(events));

        *ppInfo = pInfo;

        ShowWindow(hwnd, SW_SHOW);
        UpdateWindow(hwnd);

        return true;
    }

    void PlatformDestroyWindow(WindowInfo** ppInfo)
    {
		WindowInfo*& pInfo {*ppInfo};

        DE_CORE_ASSERT(pInfo->handle, "Failed to get window handle!");
        DE_CORE_ASSERT(pInfo->context, "Failed to get device context!");

        auto* events {reinterpret_cast<WindowEventCallback*>(GetWindowLongPtrW(pInfo->handle, GWLP_USERDATA))};
        if (events)
        {
            delete events;
        }

        SetWindowLongPtrW(pInfo->handle, GWLP_USERDATA, 0);
        ReleaseDC(pInfo->handle, pInfo->context);
        DestroyWindow(pInfo->handle);

        pInfo->handle  = nullptr;
        pInfo->context = nullptr;

        delete pInfo;
    }

    void PlatformShutdown()
    {
        UnregisterClassW(L"DropWindowClass", g_hInstance);
    }

    bool PlatformPollEvents()
    {
        MSG msg {};
        while (PeekMessageW(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessageW(&msg);
            if (msg.message == WM_QUIT)
            {
                return false;
            }
        }

        return true;
    }

    void PlatformResizeWindow(const WindowInfo* pInfo, const WindowSize* pSize)
    {
        i32 clientWidth {pSize->clientRect.right - pSize->clientRect.left};
        i32 clientHeight {pSize->clientRect.bottom - pSize->clientRect.top};

        LONG_PTR dwStyle {GetWindowLongPtrW(pInfo->handle, GWL_STYLE)};
        LONG_PTR exStyle {GetWindowLongPtrW(pInfo->handle, GWL_EXSTYLE)};

        RECT rc {0, 0, clientWidth, clientHeight};
        AdjustWindowRectEx(&rc, (DWORD) dwStyle, false, (DWORD) exStyle);

        SetWindowPos(pInfo->handle, nullptr, pSize->fullRect.left, pSize->fullRect.top, rc.right - rc.left, rc.bottom - rc.top, SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOSENDCHANGING);
    }

	void PlatformSendQuit(i32 exitCode)
	{
		PostQuitMessage(exitCode);
	}

} // namespace Drop
#endif // DE_PLATFORM_WINDOWS