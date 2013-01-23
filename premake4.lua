solution "GMad"

	language "C++"
	location ( "/" )
	flags { "Symbols", "NoEditAndContinue", "NoPCH", "StaticRuntime", "EnableSSE" }
	targetdir ( "../../../../game/bin/" )
	includedirs { "../../../Bootil/include/", "../../public/" }
	libdirs { "../../../lib/public/", "../../../Bootil/lib/"..os.get().."/".._ACTION }
	
--	local SteamLib = "steam_api"
	
	
--	if os.is( "linux" ) then
--		libdirs { "../../../lib/linux32/" }
--		SteamLib = "libsteam_api"
--	end
	
--	if os.is( "macosx" ) then
--		libdirs { "../../../lib/osx32/" }
--		SteamLib = "libsteam_api"
--	end
	
	if os.is( "linux" ) or os.is( "macosx" ) then
		buildoptions { "-fPIC" }
		linkoptions  { "-fPIC" }
	end
	
	configurations
	{ 
		"Release"
	}
	
configuration "Release"
	defines { "NDEBUG" }
	flags{ "OptimizeSpeed", "FloatFast" }

project "GMad"
	uuid ( "AB8E7B19-A70C-4737-88DE-F02160767C2E" )
	files { "src/**.*" }
	kind "ConsoleApp"
	targetname( "gmad" )
	if os.is( "linux" ) then
		targetname( "gmad_linux" )
	end
	if os.is( "macosx" ) then
		targetname( "gmad_osx" )
	end
	links( { "bootil_static" } )