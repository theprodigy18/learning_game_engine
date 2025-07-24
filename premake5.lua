workspace "DropEngine"
	architecture "x64"
	configurations {"Debug", "Release", "Dist"}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "Engine"
	location "Engine"
	kind "SharedLib"
	language "C++"
	cppdialect "C++17"

	targetdir ("bin/" .. outputdir .. "/data")
	objdir ("bin-int/" .. outputdir .. "/data")

	pchheader "depch.h"
	pchsource "%{prj.name}/depch.cpp"

	files
	{
		"%{prj.name}/**.h",
		"%{prj.name}/*.cpp",
		"%{prj.name}/src/*.cpp"
	}

	includedirs
	{
		"%{prj.name}/include",
		"%{prj.name}"
	}


	filter "system:windows"
		staticruntime "On"
		systemversion "latest"

		defines 
		{
			"DE_PLATFORM_WINDOWS",
			"DE_BUILD_DLL"
		}

		files 
		{
			"%{prj.name}/src/Platform/Windows/**.cpp"
		}

	filter "configurations:Debug"
		defines "DE_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "DE_RELEASE"
		optimize "On"

	filter "configurations:Dist"
		defines "DE_DIST"
		optimize "On"
		