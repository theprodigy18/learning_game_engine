#pragma once

#ifdef DE_PLATFORM_WINDOWS
	#ifdef DE_BUILD_DLL
		#define DE_API __declspec(dllexport)
	#else
		#define DE_API __declspec(dllimport)
	#endif // DE_BUILD_DLL
#else 
#error "Platform not supported"
#endif // DE_PLATFORM_WINDOWS