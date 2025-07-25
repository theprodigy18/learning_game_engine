#include "Drop.h"


int main(int argc, char** argv)
{
    Drop::EngineInit();

	Drop::PlatformCreateWindow("Drop Engine", 1280, 720, 0);

	while (true)
    {

    }

	Drop::EngineShutdown();
    return 0;
}