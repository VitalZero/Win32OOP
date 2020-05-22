#pragma once
#include "stdafx.h"
#include "Resource.h"
#include <cassert>

template <class DERIVED_T>
class BaseWindow
{
public:
	BaseWindow() 
		: hWnd(nullptr), hInst(GetModuleHandle(nullptr)), wcx({0}) 
	{
	}

	BaseWindow(LPCSTR lpWindowName, DWORD dwStyle, DWORD dwExStyle = 0,
		int x = CW_USEDEFAULT, int y = CW_USEDEFAULT, int width_in = CW_USEDEFAULT, int height_in = CW_USEDEFAULT,
		HWND hWndParent = 0, HMENU hMenu = 0)
		: hWnd(nullptr), hInst(GetModuleHandle(nullptr)), wcx({ 0 })
	{
		Create(lpWindowName, dwStyle, dwExStyle, x, y, width_in, height_in, hWndParent, hMenu);
	}
	BaseWindow(const BaseWindow&) = delete;
	virtual ~BaseWindow() {}

	BOOL Create(LPCSTR lpWindowName, DWORD dwStyle, DWORD dwExStyle = 0,
		int x = CW_USEDEFAULT, int y = CW_USEDEFAULT, int width_in = CW_USEDEFAULT, int height_in = CW_USEDEFAULT,
		HWND hWndParent = 0, HMENU hMenu = 0)
	{
		RECT wr = {0};
		wr.left = x;
		wr.right = width_in + x;
		wr.top = y;
		wr.bottom = height_in + y;
		AdjustWindowRect(&wr, dwStyle, TRUE);

		width = wr.right - wr.left;
		height = wr.bottom - wr.top;
		
		wcx.cbSize = sizeof(WNDCLASSEX);
		wcx.hInstance = hInst;
		wcx.lpszClassName = lpWindowName;//ClassName();
		wcx.lpfnWndProc = DERIVED_T::WndProc;
		wcx.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
		wcx.hCursor = LoadCursor(nullptr, IDC_ARROW);
		wcx.hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_MAINICON));

		ATOM aResult = RegisterClassEx(&wcx);
		assert(aResult != 0);

		hWnd = CreateWindowEx(
			dwExStyle, /*ClassName()*/lpWindowName, lpWindowName, dwStyle,
			x, y, width, height,
			hWndParent, hMenu, hInst, this
		);

		assert(hWnd != 0);

		return (hWnd ? TRUE : FALSE);
	}

	void Show(int nCmdShow) { ShowWindow(hWnd, nCmdShow); }
	HWND Window() const { return hWnd; }

protected:
	virtual LPCSTR ClassName() const = 0;
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) = 0;

public:
	static constexpr int nMaxLoadString = 100;

protected:
	HWND hWnd;
	HINSTANCE hInst;
	char szClassName[nMaxLoadString];
	int width;
	int height;
	WNDCLASSEX wcx;

public:
	static LRESULT CALLBACK WndProc(HWND hWndProc, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		DERIVED_T* pThis = nullptr;

		if (uMsg == WM_NCCREATE)
		{
			LPCREATESTRUCT pCreate = (LPCREATESTRUCT)lParam;
			pThis = (DERIVED_T*)pCreate->lpCreateParams;
			pThis->hWnd = hWndProc;

			assert(pThis != nullptr);

			SetWindowLongPtr(hWndProc, GWLP_USERDATA, (LONG_PTR)pThis);
		}
		else
		{
			pThis = (DERIVED_T*)GetWindowLongPtr(hWndProc, GWLP_USERDATA);
		}
		if (pThis)
		{
			return pThis->HandleMessage(uMsg, wParam, lParam);
		}
		else
		{
			return DefWindowProc(hWndProc, uMsg, wParam, lParam);
		}
	}
};