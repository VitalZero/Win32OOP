#include "ReportWindow.h"
#include <algorithm>
#include <winspool.h>
#include <Uxtheme.h>
#include <vsstyle.h>
#include <vssym32.h>
#include <memory>
#pragma comment(lib, "uxtheme.lib")

LRESULT ReportWindow::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CREATE:
		
		lockers.clear();
		lm.GetLockers(lockers);
		Print();
		
		return 0;

	case WM_CLOSE:
		DestroyWindow(hWnd);
		UnregisterClass(ClassName(), hInst);
		break;

	case WM_PAINT:
		Paint();
		return 0;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

void ReportWindow::Print()
{

	ULONG_PTR gdiPlusToken = 0;
	Gdiplus::GdiplusStartupInput gdiplusStartupInput = { 0 };

	Gdiplus::GdiplusStartup(&gdiPlusToken, &gdiplusStartupInput, nullptr);
	char szPrinterName[256] = { 0 };
	unsigned long nBufferSize = 255; // DWORD = unsigned long
	
	if (GetDefaultPrinter(szPrinterName, &nBufferSize))
	{
		HDC hdcPrint = nullptr;
		hdcPrint = CreateDC(nullptr, szPrinterName, nullptr, nullptr);
	
		/*int nDeviceCapRet = DeviceCapabilities(szPrinterName, nullptr, DC_PAPERSIZE, nullptr, nullptr);
		auto ptPoints = std::make_unique<POINT[]>(nDeviceCapRet);
		nDeviceCapRet = DeviceCapabilities(szPrinterName, nullptr, DC_PAPERSIZE, (LPSTR)&ptPoints[0], nullptr);

		
		int nPxWidth = 0;
		int nPxHeight = 0;
		nPxWidth = GetDeviceCaps(hdcPrint, HORZRES);
		nPxHeight = GetDeviceCaps(hdcPrint, VERTRES);

		SetWindowPos(Window(), 0, 0, 0, ptPoints[0].x/4, ptPoints[0].y/4, SWP_NOMOVE | SWP_NOZORDER);*/

		DOCINFO di = { 0 };
		di.lpszDocName = "Reporte de lockers";
		di.cbSize = sizeof(di);

		StartDoc(hdcPrint, &di);
		StartPage(hdcPrint);

		//Gdiplus::Graphics* gfx = new Gdiplus::Graphics(hdcPrint);
		auto gfx = std::make_unique<Gdiplus::Graphics>(hdcPrint);
		gfx->SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);

		//Gdiplus::Pen* pen = new Gdiplus::Pen(Gdiplus::Color(0, 0, 0));
		auto pen = std::make_unique<Gdiplus::Pen>(Gdiplus::Color(0, 0, 0));
		//pen->SetWidth(1);

		//Gdiplus::SolidBrush* solidBrush = new Gdiplus::SolidBrush(Gdiplus::Color(0, 0, 0));
		auto solidBrush = std::make_unique<Gdiplus::SolidBrush>(Gdiplus::Color(0, 0, 0));

		Gdiplus::FontFamily fontFamily(L"Arial");
		Gdiplus::Font font(&fontFamily, 8, Gdiplus::FontStyleRegular, Gdiplus::UnitPoint);
		Gdiplus::Font fontBold(&fontFamily, 8, Gdiplus::FontStyleBold, Gdiplus::UnitPoint);


		int x = 10;
		int y = 10;
		int w[3] = { 250, 80, 250 };
		int h = 16;
		const int padding = 3;

		std::wstring data;
		std::string source;

		for (unsigned int i = 0; i < 30; i++)
		{
			for (unsigned j = 0; j < 3; j++)
			{
				gfx->DrawRectangle(pen.get(), x, y, w[j], h);
				x += w[j];
			}
			x = 10;
			if (i == 0)
			{
				
				gfx->DrawString(L"USUARIO", -1, &fontBold, { (float)(x + padding), (float)y }, solidBrush.get());
				x += w[0];
				gfx->DrawString(L"FECHA", -1, &fontBold, { (float)(x + padding), (float)y }, solidBrush.get());
				x += w[1];
				gfx->DrawString(L"USUARIO ANTERIOR", -1, &fontBold, { (float)(x + padding), (float)y }, solidBrush.get());
				x = 10;
				y += h;
				continue;
			}

			source = lockers.at(i-1).GetAssignedUser();
			data.resize(source.length(), L' ');
			std::copy(source.begin(), source.end(), data.begin());

			gfx->DrawString(data.c_str(), -1, &font, { (float)(x + padding), (float)y }, solidBrush.get());
			x += w[0];

			source = lockers.at(i - 1).GetDate();
			data.resize(source.length(), L' ');
			std::copy(source.begin(), source.end(), data.begin());

			gfx->DrawString(data.c_str(), -1, &font, { (float)(x + padding), (float)y }, solidBrush.get());
			x += w[1];

			source = lockers.at(i - 1).GetPreviousUser();
			data.resize(source.length(), L' ');
			std::copy(source.begin(), source.end(), data.begin());

			gfx->DrawString(data.c_str(), -1, &font, { (float)(x + padding), (float)y }, solidBrush.get());

			x = 10;
			y += h;
		}

		//delete gfx;
		//delete pen;
		//delete solidBrush;

		EndPage(hdcPrint);
		EndDoc(hdcPrint);
		
		DeleteDC(hdcPrint);
	}
	else
	{
		MessageBox(hWnd, "No se pudo obtener la impresora", "Error", MB_ICONERROR);
	}
	Gdiplus::GdiplusShutdown(gdiPlusToken);
}

void ReportWindow::Paint()
{
	PAINTSTRUCT ps = { 0 };
	HDC hdc = BeginPaint(hWnd, &ps);
	//Paint	
	HTHEME hTheme = nullptr;
	hTheme = OpenThemeData(hWnd, L"Button");
	RECT rc;
	rc.top = 10;
	rc.left = 10;
	rc.right = rc.left + 75;
	rc.bottom = rc.top + 24;
	
	DrawThemeBackground(hTheme, hdc, BP_PUSHBUTTON, PBS_HOT, &rc, nullptr);

	rc.top = 44;
	rc.left = 10;
	rc.right = rc.left + 75;
	rc.bottom = rc.top + 24;

	DrawThemeBackground(hTheme, hdc, BP_PUSHBUTTON, PBS_DEFAULTED, &rc, nullptr);

	//No Paint
	EndPaint(hWnd, &ps);
}