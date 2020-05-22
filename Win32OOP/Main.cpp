// Win32OOP.cpp : Define el punto de entrada de la aplicación.
//

#include "stdafx.h"
#include "App.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
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