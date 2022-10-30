#pragma once

#include "Core/Application.h"

extern Silver::Application* Silver::CreateApplication(int ArgC, char** ArgV);

// TODO(Milan): WinMain

int main(int argc, char** argv)
{
	Silver::Application* app = Silver::CreateApplication(argc, argv);
	app->Run();
	delete app;
}