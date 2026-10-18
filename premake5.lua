workspace "Terraria"
    architecture "x64"
    startproject "Terraria"

    configurations{
        "Debug",
        "Release",
		"Dist"
    }

    outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

    IncludeDir = {}
    IncludeDir["GLFW"] = "vendor/GLFW/include"
    IncludeDir["glad"] = "vendor/glad/include"
    IncludeDir["ImGui"] = "vendor/imgui"
    IncludeDir["glm"] = "vendor/glm"
    IncludeDir["stb_image"] = "vendor/stb_image/include"
    IncludeDir["FastNoiseLite"] = "vendor/FastNoiseLite/include"
    
group "Dependencies"
	include "vendor/GLFW"
	include "vendor/glad"
	include "vendor/imgui"
	include "vendor/stb_image"
group ""
        
project "Engine"
	location "Engine"
	kind "StaticLib"
	language "C++"
	cppdialect "C++23"
	staticruntime "on"
    buildoptions { "/utf-8" }
	
    pchheader "pch.h"
    pchsource "Engine/src/pch.cpp"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
	
	files{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs{
		"%{prj.name}/src",
		"vendor/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.ImGui}/backends",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb_image}",
		"%{IncludeDir.FastNoiseLite}"
	}

	links{ 
		"GLFW",
		"glad",
		"ImGui",
		"stb_image",
		"opengl32.lib"
	}
	
	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		defines "DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "DIST"
		runtime "Release"
		optimize "on"
		

project "Terraria"
    location "Terraria"
    kind "ConsoleApp"
	cppdialect "C++23"
	staticruntime "on"
	buildoptions "/utf-8"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs{
		"%{prj.name}/src",
		"Engine/src",
		"vendor/spdlog/include",

		"%{IncludeDir.GLFW}",
		"%{IncludeDir.glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.ImGui}/backends",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb_image}",
		"%{IncludeDir.FastNoiseLite}"
	}

	links{
		"Engine"
	}

	postbuildcommands{
		"{COPYDIR} \"%{prj.location}/Assets\" \"%{cfg.targetdir}/Assets\""
	}

	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		defines "DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "DIST"
		runtime "Release"
		optimize "on"
