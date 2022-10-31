#pragma once

#include "Core/Application.h"
#include "Core/Logging.h"

extern Silver::Application* Silver::CreateApplication(int ArgC, char** ArgV);

// TODO(Milan): WinMain

int main(int argc, char** argv)
{
	Silver::Log::Init();
	Silver::Application* app = Silver::CreateApplication(argc, argv);
	app->Run();
	delete app;
	Silver::Log::Shutdown();
}