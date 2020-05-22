#include "Button.h"

Button::Button()
    :
    hWnd(nullptr), x(0), y(0), id(0), pressed(false)
{
}

void Button::Create(LPCSTR lpszText, DWORD dwStyle, int x_in, int y_in, HWND hWndParent, int id_in)
{
	assert(hWnd == nullptr);

    x = x_in;
    y = y_in;
    id = id_in;

    hWnd = CreateWindow("Button", lpszText, dwStyle, x, y, width, height, hWndParent, (HMENU)id, GetModuleHandle(nullptr), 0);

    assert(hWnd != 0);
}

void Button::Create(int idString, DWORD dwStyle, int x_in, int y_in, HWND hWndParent, int id_in)
{
	char szButtonName[nMaxLoadString] = {};
	LoadString(GetModuleHandle(nullptr), idString, szButtonName, nMaxLoadString);

	Create(szButtonName, dwStyle, x_in, y_in, hWndParent, id_in);
}

HWND Button::Hwnd() const
{
    return hWnd;
}

void Button::WasPressed(bool pressed) 
{
	this->pressed = pressed;
}

bool Button::WasPressed() const
{
	return pressed;
}