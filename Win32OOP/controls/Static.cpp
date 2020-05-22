#include "Static.h"

Static::Static()
	:
	hWnd(nullptr), x(0), y(0), width(0), id(0)
{
}

void Static::Create(LPCSTR lpszText, DWORD dwStyle, int x_in, int y_in, int width_in, HWND hWndParent, int id_in)
{
	assert(hWnd == nullptr);

	x = x_in;
	y = y_in;
	id = id_in;
	width = width_in;

	hWnd = CreateWindow("Static", lpszText, dwStyle, x, y, width, height, hWndParent, (HMENU)id, GetModuleHandle(nullptr), 0);

	assert(hWnd != 0);
}

void Static::Create(int idString, DWORD dwStyle, int x_in, int y_in, int width_in, HWND hWndParent, int id_in)
{
	char szStaticText[nMaxLoadString] = {};
	LoadString(GetModuleHandle(nullptr), idString, szStaticText, nMaxLoadString);

	Create(szStaticText, dwStyle, x_in, y_in, width_in, hWndParent, id_in);
}

void Static::SetText(const char * szText)
{
	SetWindowText(hWnd, szText);
}

HWND Static::Hwnd() const
{
	return hWnd;
}
