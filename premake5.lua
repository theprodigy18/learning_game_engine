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
		"%{prj.name}",
		"%{prj.name}/vendor/spdlog/include"
	}
	libdirs
	{
		os.findlib("win32")
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

		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir)
		}

	filter "configurations:Debug"
		defines "DE_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "DE_RELEASE"
		optimize "On"

	filter "configurations:Dist"
		defines 
		{
			"DE_DIST",
			"DE_NO_ASSERT"
		}
		optimize "On"

	filter "action:vs*"
		buildoptions {"/utf-8"}
		

project "Editor"
	location "Editor"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"

	targetdir ("bin/" .. outputdir)
	objdir ("bin-int/" .. outputdir)

	files 
	{
		"%{prj.name}/src/*.cpp"
	}
	includedirs
	{
		"Engine",
		"Engine/vendor/spdlog/include"
	}
	links
	{
		"Engine"
	}

	filter "system:windows"
		staticruntime "On"
		systemversion "latest"

		defines "DE_PLATFORM_WINDOWS"

	filter "configurations:Debug"
		defines "DE_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "DE_RELEASE"
		optimize "On"

	filter "configurations:Dist"
		defines 
		{
			"DE_DIST",
			"DE_NO_ASSERT"
		}
		optimize "On"

	filter "action:vs*"
		buildoptions {"/utf-8"}