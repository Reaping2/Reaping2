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
    files  { "../src/main/**.h", "../src/main/**.cpp" }
 
    configuration { "Debug*" }
      defines { "_DEBUG", "DEBUG" }
      flags   { "Symbols" }
 
    configuration { "Release*" }
      defines { "NDEBUG" }
      flags   { "Optimize" }
