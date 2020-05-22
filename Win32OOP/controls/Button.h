#ifndef _BUTTON_H_
#define _BUTTON_H_

#include "../stdafx.h"
#include <cassert>

class Button
{
public:
    Button();
    void Create(LPCSTR lpszText, DWORD dwStyle, int x_in, int y_in, HWND hWndParent, int id_in);
	void Create(int idString, DWORD dwStyle, int x_in, int y_in, HWND hWndParent, int id_in);
    HWND Hwnd() const;
	void WasPressed(bool pressed);
	bool WasPressed() const;

public:

private:
    HWND hWnd;
    int id;
    int x;
    int y;
	bool pressed;
    static constexpr int width = 75;
    static constexpr int height = 24;
	static constexpr int nMaxLoadString = 100;
};

#endif