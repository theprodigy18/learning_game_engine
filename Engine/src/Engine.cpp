#include "depch.h"
#include "Engine.h"

#include "Window.h"
#include "Graphics.h"

namespace Drop
{
	bool EngineInit() 
	{ 
		LogInit();
		if (!PlatformInit())
		{
			DE_CORE_ASSERT(false, "Failed to initialize platform!");
			return false;
		}
		return true; 
	}

	void EngineShutdown() 
	{
		PlatformShutdown();
	}
} // namespace Drop