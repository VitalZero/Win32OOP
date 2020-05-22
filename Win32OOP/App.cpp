#include "App.h"

App::App()
	: hInst(GetModuleHandle(nullptr))
{
	char szTitle[MainWindow::nMaxLoadString];
	LoadString(hInst, IDS_APPTITLE, szTitle, MainWindow::nMaxLoadString);

	win = new MainWindow();
	win->Create(szTitle, WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX & ~WS_SIZEBOX, 0, CW_USEDEFAULT, CW_USEDEFAULT, 542, 400); //343
	win->Show(SW_SHOWDEFAULT);
}

App::~App()
{
	delete win;
}

int App::Run()
{
	MSG msg = {};

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}