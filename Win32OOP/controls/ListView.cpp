#include "ListView.h"
#include <vector>

ListView::ListView()
    :
    hWnd(nullptr), x(0), y(0), width(0), height(0), id(0), nTotalItems(0)/*, lvi({0}), lvc({0})*/
{
	
    INITCOMMONCONTROLSEX icex;
    icex.dwSize = sizeof(icex);
    icex.dwICC = ICC_LISTVIEW_CLASSES;

    InitCommonControlsEx(&icex);
}

void ListView::Create(DWORD dwStyle, int x_in, int y_in, int width_in, int height_in, HWND hWndParent, int id_in)
{
	assert(hWnd == nullptr);

    x = x_in;
    y = y_in;
    width = width_in;
    height = height_in;
    id = id_in;

    hWnd = CreateWindow(WC_LISTVIEW, "", dwStyle, x, y, width, height, hWndParent, (HMENU)id, GetModuleHandle(nullptr), nullptr);

    assert(hWnd != 0);
}

BOOL ListView::InsertRows(int nRows)
{
	LVITEM lvi = {};

    lvi.mask = LVIF_TEXT | LVIF_STATE | LVIF_IMAGE;
	lvi.pszText = LPSTR_TEXTCALLBACK;
	lvi.iSubItem = 0;
	lvi.stateMask = 0;
	lvi.state = 0;

    for(int i = 0; i < nRows; ++i)
    {
        lvi.iItem = i;

        if(ListView_InsertItem(hWnd, &lvi) == -1)
            return FALSE;
    }

	nTotalItems = nRows;

    return TRUE;
}

BOOL ListView::InsertColumns(int nCols)
{
	LVCOLUMN lvc = {};
	char textCol[] = "Columna";
    lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
    lvc.cx = 100;
	//lvc.pszText = textCol;
	lvc.fmt = LVCFMT_LEFT;

    for(int i = 0; i < nCols; ++i)
    {
        lvc.iSubItem = i;
		lvc.pszText = buff[i];

        if(ListView_InsertColumn(hWnd, i, &lvc) == -1)
            return FALSE;
    }

    return TRUE;
}

BOOL ListView::InsertColumn(int iCol, char * szText, int nWidthCol)
{
	LVCOLUMN lvc = {};
	lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	lvc.cx = nWidthCol;
	lvc.pszText = szText;
	lvc.iSubItem = iCol;
	lvc.fmt = LVCFMT_LEFT;

	if (ListView_InsertColumn(hWnd, iCol, &lvc) == -1)
		return FALSE;

	return TRUE;
}

void ListView::SetSubItemText(int nRow, int nCol, const std::string& strText)
{
	std::vector<char> tmpChar(strText.begin(), strText.end());
	tmpChar.push_back('\0');

	ListView_SetItemText(hWnd, nRow, nCol, &tmpChar[0]);
}


void ListView::SetSubItemText(int nRow, int nCol, char * szText)
{
	ListView_SetItemText(hWnd, nRow, nCol, szText);
}

std::string ListView::GetSubItemText(int nRow, int nCol)
{
	char tmpBuffer[256] = {};
	ListView_SetItemText(hWnd, nRow, nCol, tmpBuffer);

	return std::string(tmpBuffer);
}

void ListView::SetExStyle(DWORD dwExStyle)
{
    ListView_SetExtendedListViewStyle(hWnd, dwExStyle);
}

void ListView::SetImageList(HIMAGELIST hImage, int iType)
{
	ListView_SetImageList(hWnd, hImage, iType);
}

int ListView::GetNextSelectedItem() const
{
	if (ListView_GetSelectedCount(hWnd) > 0)
		return ListView_GetNextItem(hWnd, -1, LVNI_SELECTED);
	else
		return -1;
}

void ListView::Clear()
{
	ListView_DeleteAllItems(hWnd);
}

void ListView::GetSubItemText(int iRow, int iCol, char* szText)
{
	ListView_GetItemText(hWnd, iRow, iCol, szText, 256);
}

HWND ListView::Hwnd() const
{
    return this->hWnd;
}