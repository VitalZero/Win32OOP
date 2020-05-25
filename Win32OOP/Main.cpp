// Win32OOP.cpp : Define el punto de entrada de la aplicación.
//

#include "stdafx.h"
#include "App.h"

int APIENTRY wWinMain( HINSTANCE hInstance,
                      HINSTANCE hPrevInstance,
                      LPWSTR    lpCmdLine,
                      int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
	try
	{
		App app;
		return app.Run();
	}
	catch (std::runtime_error& e)
	{
		MessageBox(nullptr, e.what(), "Error!", MB_ICONERROR);
	}
	return 0;
}