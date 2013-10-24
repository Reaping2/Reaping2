solution "Reaping2"
  location "../build"
  configurations { "Debug", "Release" }
  configuration { "Debug" }
    targetdir "../bin/debug"
  configuration { "Release" }
    targetdir "../bin/release"
 
  if _ACTION == "clean" then
    os.rmdir("../bin")
  end
   
  project "main"
    language "C++"
    kind     "ConsoleApp"
	libdirs { "../deps/boost_1_54_0/stage/lib", "../deps/glfw-3.0.3/build/src/Release", "../deps/glfw-3.0.3/build/src/Debug", "../deps/glfw-3.0.3/build/src" }
	includedirs { "../src", "../deps/boost_1_54_0", "../deps/glfw-3.0.3/include" }
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
 
    configuration { "Release*" }
      defines { "NDEBUG" }
      flags   { "Optimize" }
