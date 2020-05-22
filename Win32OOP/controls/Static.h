#pragma once

#include "../stdafx.h"
#include <cassert>

class Static
{
public:
	Static();
	void Create(LPCSTR lpszText, DWORD dwStyle, int x_in, int y_in, int width_in, HWND hWndParent, int id_in = -1);
	void Create(int idString, DWORD dwStyle, int x_in, int y_in, int width_in, HWND hWndParent, int id_in = -1);
	void SetText(const char* szText);
	HWND Hwnd() const;

public:

private:
	HWND hWnd;
	int id;
	int x;
	int y;
	int width;
	static constexpr int height = 30;
	static constexpr int nMaxLoadString = 100;
};