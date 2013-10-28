dofile("premake-helper.lua")

solution "Reaping2"
	location "../build"

	
	libdirs { "../deps/boost_1_54_0/stage/lib", "../deps/glfw-3.0.3/build/src/Release", "../deps/glfw-3.0.3/build/src/Debug", "../deps/glfw-3.0.3/build/src", "../deps/zlib-1.2.8/build/Debug", "../deps/zlib-1.2.8/build/Release" }
	includedirs { "../src", "../deps/boost_1_54_0", "../deps/glfw-3.0.3/include", "../deps/glm_0_9_4_6", "../deps/zlib-1.2.8", "../deps/zlib-1.2.8/build" }
	links { "glfw3" }
	
	
	configurations { "Debug", "Release" }
	
	configuration { "Debug" }
		targetdir "../bin/debug"
	configuration { "Release" }
		targetdir "../bin/release"
 

	configuration { "Debug*" }
		defines { "_DEBUG", "DEBUG" }
		flags	 { "Symbols" }
		if os.is("windows") then
			links { "zlibstaticd" }
		end

	configuration { "Release*" }
		defines { "NDEBUG" }
		optimize "Full"
		if os.is("windows") then
			links { "zlibstatic" }
		end

	newaction 
	{
		trigger = "clean",
		description = "Cleaning up",
		execute = function()
			os.execute("echo Cleaning up stuff")
			os.rmdir("../bin")
		end
	}
	 

	project "core"
		language "C++"
		kind "StaticLib"
		setup_files_for_project("core")
	 
	project "input"
		language "C++"
		kind "StaticLib"
		setup_files_for_project("input")
	 
	project "platform"
		language "C++"
		kind "StaticLib"
		setup_files_for_project("platform")

	project "render"
		language "C++"
		kind "StaticLib"
		setup_files_for_project("render")
		
	project "main"
		language "C++"
		kind "ConsoleApp"
			
		if os.is("windows") then
		-- no boost here, the headers do this job with visual studio
			links { "opengl32" }
			linkoptions	{ "/nodefaultlib:libmsvcrt.lib", "/nodefaultlib:libmsvcrtd.lib" }
		else
			links { "Xi", "Xrandr", "GL", "boost_system" }
		end
		links { "core", "input", "platform", "render"}
		setup_files_for_project("main")

				
	project "r2pkg"
		kind     "ConsoleApp"
		language "C++"
		libdirs { "../deps/boost_1_54_0/stage/lib", "../deps/zlib-1.2.8/build/Debug", "../deps/zlib-1.2.8/build/Release" }
		includedirs { "../tools/r2pkg", "../src", "../deps/boost_1_54_0", "../deps/zlib-1.2.8", "../deps/glm_0_9_4_6", "../deps/zlib-1.2.8/build" }
		if os.is("windows") then
			-- no boost here, the headers do this job with visual studio
			links { "opengl32" }
			linkoptions  { "/nodefaultlib:libmsvcrt.lib", "/nodefaultlib:libmsvcrtd.lib" }
		else
			links { "boost_system" }
		end
		files  { "../tools/r2pkg/**.h", "../tools/r2pkg/**.cpp", "../src/platform/**.h", "../src/platform/**.cpp" }
	 
		configuration { "Debug*" }
			defines { "_DEBUG", "DEBUG" }
			flags   { "Symbols" }
			if os.is("windows") then
				links { "zlibstaticd" }
			end
	 
		configuration { "Release*" }
			defines { "NDEBUG" }
			flags   { "Optimize" }
			if os.is("windows") then
				links { "zlibstatic" }
			end
