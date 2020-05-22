#ifndef _LISTVIEW_H_
#define _LISTVIEW_H_

#include "../stdafx.h"
#include <CommCtrl.h>
#include <cassert>
#include <string>

class ListView
{
public:
    ListView();
    void Create(DWORD dwStyle, int x_in, int y_in, int width_in, int height_in, HWND hWndParent, int id_in);
    BOOL InsertRows(int nRows);
    BOOL InsertColumns(int nCols);
	BOOL InsertColumn(int iCol, char* szText, int nWidthCol = 50);
    void SetSubItemText(int nRow, int nCol, const std::string& strText);
    void SetSubItemText(int nRow, int nCol, char* szText);
    std::string GetSubItemText(int nRow, int nCol);
	void GetSubItemText(int iRow, int iCol, char* szText);
	int GetNextSelectedItem() const;
	void SetExStyle(DWORD dwExStyle);
	void SetImageList(HIMAGELIST hImage, int iType);
	void Clear();
    HWND Hwnd() const;
	char buff[5][255] = {
		{"Col 0"},
		{"Col 1"},
		{"Col 2"},
		{"Col 3"},
		{"Col 4"}
	};
public:

private:
    HWND hWnd;
    int id;
    int x;
    int y;
    int width;
    int height;
    int nTotalItems;
};

#endif