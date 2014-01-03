dofile("premake-helper.lua")

solution "Reaping2"
	location "../build"
	
	libdirs {
		"../deps/boost_1_54_0/stage/lib",
		"../deps/glfw-3.0.3/build/src/Release",
		"../deps/glfw-3.0.3/build/src/Debug",
		"../deps/glfw-3.0.3/build/src",
		"../deps/zlib-1.2.8/build/Debug",
		"../deps/zlib-1.2.8/build/Release",
		"../deps/lpng166/build",
		"../deps/lpng166/build/Debug",
		"../deps/lpng166/build/Release",
		"../deps/libogg-1.3.1/build/lib",
		"../deps/libogg-1.3.1/lib/debug",
		"../deps/libogg-1.3.1/lib/release",
		"../deps/libvorbis-1.3.3/bin/lib",
		"../deps/libvorbis-1.3.3/bin/lib/debug",
		"../deps/libvorbis-1.3.3/bin/lib/release",
		"../deps/portaudio_v19/build_dir/bin/Win32/Debug",
		"../deps/portaudio_v19/build_dir/bin/Win32/Release",
		"../deps/portaudio_v19/build_dir/lib",
		"../deps/glew-1.10.0/lib"
	}
	includedirs {
		"../src",
		"../deps/boost_1_54_0",
		"../deps/glfw-3.0.3/include",
		"../deps/glm_0_9_4_6",
		"../deps/zlib-1.2.8",
		"../deps/zlib-1.2.8/build",
		"../deps/lpng166",
		"../deps/json-cpp-0.6.0-rc2",
		"../deps/libogg-1.3.1/include",
		"../deps/libvorbis-1.3.3/include",
		"../deps/portaudio_v19/build_dir/include",
		"../deps/portaudio_v19/include",
		"../deps/glew-1.10.0/include"
	}
	links { "glfw3" }
	
	-- flags { "FatalWarnings" }

	configurations { "Debug", "Release" }
	
	configuration { "Debug" }
		targetdir "../bin/debug"
	configuration { "Release" }
		targetdir "../bin/release"
 

	configuration { "Debug*" }
		defines { "_DEBUG", "DEBUG" }
		flags   { "Symbols" }
		if os.is("windows") then
			links { "opengl32", "zlibstaticd", "libpng16_staticd", "glew32sd", "libogg_static", "libvorbis_static", "libvorbisfile_static", "portaudio_static_x86" }
			linkoptions	{ "/nodefaultlib:msvcrt.lib" }
		else
			links { "X11", "Xxf86vm", "Xi", "Xrandr", "GL", "boost_system", "boost_thread", "boost_filesystem", "portaudio", "ogg", "vorbis", "vorbisfile", "GLEW", "z", "png" }
		end
 
	configuration { "Release*" }
		defines { "NDEBUG" }
		flags   { "Optimize" }
		if os.is("windows") then
			links { "opengl32", "zlibstatic", "libpng16_static", "glew32s", "libogg_static", "libvorbis_static", "libvorbisfile_static", "portaudio_static_x86" }
			linkoptions	{ "/nodefaultlib:msvcrtd.lib" }
		else
			links { "X11", "Xxf86vm", "Xi", "Xrandr", "GL", "boost_system", "boost_thread", "boost_filesystem", "portaudio", "ogg", "vorbis", "vorbisfile", "GLEW", "z", "png" }
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
		links { "json-cpp" }
		setup_files_for_project("platform")

	project "render"
		language "C++"
		kind "StaticLib"
		setup_files_for_project("render")

	project "ui"
		language "C++"
		kind "StaticLib"
		setup_files_for_project("ui")

	project "audio"
		language "C++"
		kind "StaticLib"
		setup_files_for_project("audio")

	project "json-cpp"
		language "C++"
		kind "StaticLib"
		files  { "../deps/json-cpp-0.6.0-rc2/*.cpp" }
		
	project "main"
		language "C++"
		kind "ConsoleApp"

		links { "core", "input", "platform", "render", "ui", "audio", "json-cpp" }
		setup_files_for_project("main")

		files  { "../src/**.h", "../src/**.cpp" }

	project "r2pkg"
		kind     "ConsoleApp"
		language "C++"

		links { "platform", "json-cpp" }
		if os.is("windows") then
			-- no boost here, the headers do this job with visual studio
			links { "opengl32" }
		else
			links { "boost_system", "boost_program_options", "z", "png" }
		end
		files  { "../tools/r2pkg/**.h", "../tools/r2pkg/**.cpp" }
	 
		configuration { "Debug*" }
			defines { "_DEBUG", "DEBUG" }
			flags   { "Symbols" }
			if os.is("windows") then
				links { "zlibstaticd", "libpng16_staticd" }
			end
	 
		configuration { "Release*" }
			defines { "NDEBUG" }
			flags   { "Optimize" }
			if os.is("windows") then
				links { "zlibstatic", "libpng16_static" }
			end

	project "texcombiner"
		kind     "ConsoleApp"
		language "C++"

		links { "platform", "json-cpp" }
		if os.is("windows") then
			-- no boost here, the headers do this job with visual studio
			links { "opengl32" }
		else
			links { "boost_system" }
		end
		files  { "../tools/texcombiner/**.h", "../tools/texcombiner/**.cpp" }
	 
		configuration { "Debug*" }
			defines { "_DEBUG", "DEBUG" }
			flags   { "Symbols" }
			if os.is("windows") then
				links { "zlibstaticd", "libpng16_staticd" }
			end
	 
		configuration { "Release*" }
			defines { "NDEBUG" }
			flags   { "Optimize" }
			if os.is("windows") then
				links { "zlibstatic", "libpng16_static" }
			end
