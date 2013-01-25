solution "GMad"

	newoption {
	   trigger     = "outdir",
	   value       = "bin/",
	   description = "Output directory"
	}
	
	newoption {
	   trigger     = "bootil_lib",
	   value       = "lib/",
	   description = "The folder containing the bootil lib"
	}
	
	newoption {
	   trigger     = "bootil_inc",
	   value       = "include/",
	   description = "The folder containing the bootil h"
	}
	
	language "C++"
	location ( "/" )
	flags { "Symbols", "NoEditAndContinue", "NoPCH", "StaticRuntime", "EnableSSE" }
	targetdir ( _OPTIONS.outdir )
	includedirs { "include/", _OPTIONS.bootil_inc }
	libdirs { _OPTIONS.bootil_lib }
		
	if os.is( "linux" ) or os.is( "macosx" ) then
		buildoptions { "-fPIC" }
		linkoptions  { "-fPIC" }
	end
	
	if os.is( "linux" ) then
		links { "pthread" }
	end
	
	configurations
	{ 
		"Release"
	}
	
	configuration "Release"
		defines { "NDEBUG" }
		flags{ "OptimizeSpeed", "FloatFast" }

	project "GMad"
		uuid ( "AB8E7B19-A70C-4737-88DE-F02160737C2E" )
		files { "src/**.*", "include/**.*" }
		kind "ConsoleApp"
		targetname( "gmad" )
		
		if os.is( "linux" ) then
			targetname( "gmad_linux" )
		end
		
		if os.is( "macosx" ) then
			targetname( "gmad_osx" )
		end
		
		links( { "bootil_static" } )