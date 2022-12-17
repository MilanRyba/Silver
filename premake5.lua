include "Dependencies.lua"

workspace "Silver"
	architecture "x64"
	startproject "SilverEditor"

	configurations { "Debug", "Release", "Distribution" }

	flags { "MultiProcessorCompile" }

BinOutput = "%{wks.location}/Build/%{cfg.system}/%{cfg.buildcfg}-%{cfg.architecture}" -- Silver/Build/Windows/Debug-x64/.exe
IntOutput = "%{wks.location}/Build/%{cfg.system}/%{prj.name}.dir/%{cfg.buildcfg}" -- Silver/Build/Windows/Silver.dir/Debug/.obj

-- group dependencies
group "ThirdParty"
include "ThirdParty/GLFW"
include "ThirdParty/ImGui"
group ""

-- include projects
include "Silver"
include "SilverEditor"