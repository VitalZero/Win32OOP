#pragma once
#include "MainWindow.h"

class App
{
public:
	App();
	App(const App&) = delete;
	App& operator=(const App&) = delete;
	~App();
	int Run();

private:
	HINSTANCE hInst;
	MainWindow* win;
};