#include "Drop.h"

Drop::WindowInfo* g_pInfo {nullptr};

int main(int argc, char** argv)
{
    Drop::EngineInit();

    Drop::WindowEventCallback events {};
    events.OnClose     = ([]()
    { 
		Drop::PlatformSendQuit();
		return true; });
    events.OnSetMinMax = ([](Drop::WindowPoint& outMin, Drop::WindowPoint& outMax)
    {
		outMin.x = 800;
		outMin.y = 450;
		outMax.x = 1920;
		outMax.y = 1080;
		return true; });

    Drop::WindowInitProps props {};
    props.title  = "Drop Editor";
    props.width  = 1280;
    props.height = 720;
    props.flags  = WINDOW_TYPE_NORMAL;
    props.events = events;

    if (!Drop::PlatformCreateWindow(&g_pInfo, &props))
    {
        Drop::EngineShutdown();
        return -1;
    }

    bool running {true};
    while (running)
    {
        running = Drop::PlatformPollEvents();
    }

    Drop::PlatformDestroyWindow(&g_pInfo);
    Drop::EngineShutdown();
    return 0;
}