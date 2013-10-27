solution "Reaping2"
	location "../build"
	configurations { "Debug", "Release" }
	configuration { "Debug" }
		targetdir "../bin/debug"
	configuration { "Release" }
		targetdir "../bin/release"
    
	project "main"
		kind     "ConsoleApp"
		language "C++"
		libdirs { "../deps/boost_1_54_0/stage/lib", "../deps/glfw-3.0.3/build/src/Release", "../deps/glfw-3.0.3/build/src/Debug", "../deps/glfw-3.0.3/build/src", "../deps/zlib-1.2.8/build/Debug", "../deps/zlib-1.2.8/build/Release" }
		includedirs { "../src", "../deps/boost_1_54_0", "../deps/glfw-3.0.3/include", "../deps/glm_0_9_4_6", "../deps/zlib-1.2.8", "../deps/zlib-1.2.8/build" }
		links { "glfw3" }
		if os.is("windows") then
			-- no boost here, the headers do this job with visual studio
			links { "opengl32" }
			linkoptions  { "/nodefaultlib:libmsvcrt.lib", "/nodefaultlib:libmsvcrtd.lib" }
		else
			links { "Xi", "Xrandr", "GL", "boost_system" }
		end
		files  { "../src/**.h", "../src/**.cpp" }
	 
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
