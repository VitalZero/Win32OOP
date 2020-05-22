#pragma once
#include "MainWindow.h"
#include <fstream>

MainWindow::MainWindow() 
	: manager("basedatos.txt"), hImage(nullptr), rpt(manager)//<MainWindow>()
{
	lockers.clear();
	manager.GetLockers(lockers);

	LoadString(hInst, IDS_APPCLASSNAME, szClassName, nMaxLoadString);
}

LRESULT MainWindow::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CREATE:
	{
		OnCreate();
		return 0;
	}

	case WM_COMMAND:
		if(HIWORD(wParam) == BN_CLICKED)
			OnCommand(LOWORD(wParam));
		return 0;

	case WM_CTLCOLORSTATIC:
		return COLOR_WINDOW+1;

	case WM_NOTIFY:
		OnNotify(lParam);
		return 0;
	
	case WM_DESTROY:
		ImageList_Destroy(hImage);
		DeleteObject(hFont);
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

BOOL CALLBACK MainWindow::SetChildWndFontProc(HWND hWndChild, LPARAM font)
{
	SendMessage(hWndChild, WM_SETFONT, (WPARAM)font, TRUE);
	return TRUE;
}

void MainWindow::OnCreate()
{
	HMENU hMenu = LoadMenu(hInst, MAKEINTRESOURCE(IDR_MAINMENU));
	SetMenu(hWnd, hMenu);

	lvMain.Create(WS_CHILD | WS_VISIBLE | WS_BORDER | LVS_REPORT | LVS_ALIGNTOP | LVS_SHOWSELALWAYS | LVS_SINGLESEL,
		11, 11, 438, 379/*322*/, hWnd, ID_LISTVIEW_MAIN);
	lvMain.SetExStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_SUBITEMIMAGES );

	btnAsignar.Create(IDS_ASIGNAR, BS_PUSHBUTTON | WS_CHILD | WS_VISIBLE | BS_TEXT,
		456, 11, hWnd, ID_BUTTON_ASIGNAR);

	btnFiltro.Create(IDS_TODOS, BS_PUSHBUTTON | WS_CHILD | WS_VISIBLE | BS_TEXT | BS_SPLITBUTTON,
		456, 41, hWnd, ID_BUTTON_VERTODOS);

	btnBuscar.Create(IDS_BUSCAR, BS_PUSHBUTTON | WS_CHILD | WS_VISIBLE | BS_TEXT,
		456, 71, hWnd, ID_BUTTON_BUSCAR);

	btnEliminar.Create(IDS_ELIMINAR, BS_PUSHBUTTON | WS_CHILD | WS_VISIBLE | BS_TEXT,
		456, 101, hWnd, ID_BUTTON_ELIMINAR);

	btnAgregar.Create(IDS_AGREGAR, BS_PUSHBUTTON | WS_CHILD | WS_VISIBLE | BS_TEXT,
		456, 131, hWnd, ID_BUTTON_AGREGAR);

	lblLockers.Create("", WS_CHILD | WS_VISIBLE | SS_CENTER | SS_EDITCONTROL | SS_REALSIZECONTROL,
		456, 161, 75, hWnd, ID_STATIC_LOCKERS);

	hImage = ImageList_Create(16, 16, ILC_COLOR32, 1, 1);

	/*HINSTANCE hDllInstance = LoadLibrary("Shell32.dll");
	HICON hIcon = LoadIcon(hDllInstance, MAKEINTRESOURCE(303));
	ImageList_AddIcon(hImage, hIcon);
	DestroyIcon(hIcon);
	FreeLibrary(hDllInstance);*/

	HICON hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICONLOCKERSMALL));
	ImageList_AddIcon(hImage, hIcon);
	DestroyIcon(hIcon);

	hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICONNO));
	ImageList_AddIcon(hImage, hIcon);
	DestroyIcon(hIcon);

	lvMain.SetImageList(hImage, LVSIL_SMALL);

	const int nColsWidths[] = { 50, 250, 80, 20, 20 };

	for (int i = 0, nCols = 5; i < nCols; ++i)
	{
		char szColNames[nMaxLoadString] = {};
		LoadString(hInst, IDS_COLUMN_LOCKER + i, szColNames, nMaxLoadString);

		lvMain.InsertColumn(i, szColNames, nColsWidths[i]);
	}

	lvMain.InsertRows(lockers.size());

	lblLockers.SetText(GetLockersInfo().data());
	
	HDC hDC = GetDC(hWnd);
	int iFontSize = -MulDiv(9, GetDeviceCaps(hDC, LOGPIXELSY), 72);

	hFont = CreateFont(iFontSize, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
		OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, "Segoe UI");

	DeleteDC(hDC);

	EnumChildWindows(hWnd, SetChildWndFontProc, (LPARAM)hFont);/*GetStockObject(DEFAULT_GUI_FONT));*/
}

void MainWindow::OnCommand(WORD wCmdId)
{
	switch (wCmdId)
	{
	case ID_ARCHIVO_ACERCA:
		DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUT), hWnd, (DLGPROC)StaticAboutDlgProc);
	break;

	case ID_ARCHIVO_EXPORTARACSV:
		OnSaveFile();
	break;

	case ID_ARCHIVO_SALIR:
		DestroyWindow(hWnd);
	break;

	case ID_ARCHIVO_REPORTE:
	{
		//rpt.Create("Report", WS_OVERLAPPEDWINDOW , 0, CW_USEDEFAULT, CW_USEDEFAULT, 610, 400);
		//rpt.Show(1);
		MessageBox(hWnd, "Próximamente", "Info", MB_ICONINFORMATION);
	}
	break;

	case ID_EDITAR_ASIGNAR:
	case ID_BUTTON_ASIGNAR:
	{
		int iItem = lvMain.GetNextSelectedItem();

		if (iItem >= 0)
		{
			btnAsignar.WasPressed(true);

			if (DialogBoxParam(hInst, MAKEINTRESOURCE(IDD_INPUT_DIALOG), hWnd, (DLGPROC)StaticInputDlgProc, (LPARAM)this) == IDD_OK)
			{
				char tmpBuffer[256] = {};
				lvMain.GetSubItemText(iItem, 0, tmpBuffer);
				assert(tmpBuffer[0] != '\0');

				int lockerNum = std::stoi(tmpBuffer);
				try
				{
					manager.ChangeUser(lockerNum, szDialogInput);
					manager.GetLockers(lockers, Locker::Filter::All);
				}
				catch (std::runtime_error& rte)
				{
					MessageBox(hWnd, rte.what(), "Error!!", MB_OK | MB_ICONERROR);
				}
				lvMain.Clear();
				lvMain.InsertRows(lockers.size());

				lblLockers.SetText(GetLockersInfo().data());
			}

			btnAsignar.WasPressed(false);
		}
		else
		{
			MessageBox(hWnd, "Ningún elemento está seleccionado", "Selecciona un elemento", MB_ICONEXCLAMATION);
		}
	}
	break;

	case ID_VER_TODOS:
	case ID_BUTTON_VERTODOS:
	{
		manager.GetLockers(lockers);
		lvMain.Clear();
		lvMain.InsertRows(lockers.size());

		lblLockers.SetText(GetLockersInfo().data());
	}
	break;

	case ID_VER_ASIGNADOS:
	case ID_MENU_VERASIGNADOS:
	{
		manager.GetLockers(lockers, Locker::Filter::Assigned);
		lvMain.Clear();
		lvMain.InsertRows(lockers.size());

		lblLockers.SetText(GetLockersInfo().data());
	}
	break;

	case ID_VER_DISPONIBLES:
	case ID_MENU_VERSINASIGNAR:
	{
		manager.GetLockers(lockers, Locker::Filter::NotAssigned);
		lvMain.Clear();
		lvMain.InsertRows(lockers.size());

		lblLockers.SetText(GetLockersInfo().data());
	}
	break;

	case ID_ARCHIVO_BUSCAR:
	case ID_BUTTON_BUSCAR:
	{
		btnBuscar.WasPressed(true);

		if (DialogBoxParam(hInst, MAKEINTRESOURCE(IDD_INPUT_DIALOG), hWnd, (DLGPROC)StaticInputDlgProc, (LPARAM)this) == IDD_OK)
		{
			//std::string wordToSearch = szDialogInput;
			manager.SearchUser(szDialogInput, lockers);

			lvMain.Clear();
			lvMain.InsertRows(lockers.size());

			lblLockers.SetText(GetLockersInfo().data());
		}

		btnBuscar.WasPressed(false);
	}
	break;

	case ID_EDITAR_ELIMINARUSUARIO:
	case ID_BUTTON_ELIMINAR:
	{
		int iItem = lvMain.GetNextSelectedItem();
		if (iItem >= 0)
		{
			char tmpBuffer[256] = {};
			lvMain.GetSubItemText(iItem, 0, tmpBuffer);
			assert(tmpBuffer[0] != '\0');

			int lockerNum = std::stoi(tmpBuffer);

			try
			{
				manager.DeleteUser(lockerNum);
				manager.GetLockers(lockers, Locker::Filter::All);
			}
			catch (std::runtime_error& rte)
			{
				MessageBox(hWnd, rte.what(), "Error!!", MB_OK | MB_ICONERROR);
			}
			lvMain.Clear();
			lvMain.InsertRows(lockers.size());

			lblLockers.SetText(GetLockersInfo().data());
		}
	}
	break;

	case ID_EDITAR_AGREGAR:
	case ID_BUTTON_AGREGAR:
	{
		manager.GetLockers(lockers);
		int newLockerNumber = lockers.size() + 1;

		try
		{
			manager.AddLocker();
			manager.DeleteUser(newLockerNumber);
			manager.GetLockers(lockers);
		}
		catch (std::runtime_error& rte)
		{
			MessageBox(hWnd, rte.what(), "Error!!", MB_OK | MB_ICONERROR);
		}
		lvMain.Clear();
		lvMain.InsertRows(lockers.size());

		lblLockers.SetText(GetLockersInfo().data());
	}
	break;
	}
}

void MainWindow::OnNotify(LPARAM lParam)
{
	switch ( ((LPNMHDR)lParam)->code)
	{
	case LVN_GETDISPINFO:
	{
		if (((LPNMHDR)lParam)->hwndFrom == lvMain.Hwnd())
		{
			NMLVDISPINFO* plvdi = (NMLVDISPINFO*)lParam;

			switch (plvdi->item.iSubItem)
			{
			case 0:
			{
				char buff[100];
				strncpy_s(buff, std::to_string(lockers.at(plvdi->item.iItem).GetLockerNumber()).data(), 100);

				plvdi->item.pszText = buff;
			}
			break;

			case 1:
			{
				char buff[100];
				if (!lockers.at(plvdi->item.iItem).Enabled())
					plvdi->item.iImage = 1;

				strncpy_s(buff, lockers.at(plvdi->item.iItem).GetAssignedUser().data(), 100);
				plvdi->item.pszText = buff;
			}
			break;

			case 2:
			{
				char buff[100];
				strncpy_s(buff, lockers.at(plvdi->item.iItem).GetDate().data(), 100);
				plvdi->item.pszText = buff;
			}
			break;

			case 3:
			{
				char buff[100];
				strncpy_s(buff, lockers.at(plvdi->item.iItem).HasKeyDescription().data(), 100);
				plvdi->item.pszText = buff;
				
			}
			break;

			case 4:
			{
				char buff[100];
				strncpy_s(buff, lockers.at(plvdi->item.iItem).GetStatusDescription().data(), 100);
				plvdi->item.pszText = buff;
			}
			break;

			default:
			break;
			}
		}
	}
	break;

	case BCN_DROPDOWN:
	{
		if (((NMBCDROPDOWN*)lParam)->hdr.hwndFrom == btnFiltro.Hwnd())
		{
			RECT rcButton;
			GetClientRect(btnFiltro.Hwnd(), &rcButton);
			POINT pt;
			pt.x = rcButton.left;
			pt.y = rcButton.bottom;
			ClientToScreen(btnFiltro.Hwnd(), &pt);

			// Create a menu and add items.
			HMENU hSplitMenu = CreatePopupMenu();
			char szStringBuffer[255];

			LoadString(GetModuleHandle(nullptr), IDS_ASIGNADOS, szStringBuffer, 255);
			AppendMenu(hSplitMenu, MF_BYPOSITION, 1206, szStringBuffer);

			LoadString(GetModuleHandle(nullptr), IDS_SINASIGNAR, szStringBuffer, 255);
			AppendMenu(hSplitMenu, MF_BYPOSITION, 1207, szStringBuffer);

			// Display the menu.
			TrackPopupMenu(hSplitMenu, TPM_LEFTALIGN | TPM_TOPALIGN | TPM_VERPOSANIMATION, pt.x, pt.y, 0, hWnd, NULL);
		}
	}
	break;
	
	default:
	break;
	}
}

void MainWindow::ExportCSV(std::string& strFileName)
{
	std::ofstream csvFile;
	csvFile.open(strFileName, std::ios_base::trunc);
	
	if (!csvFile.is_open())
		throw("Error, no se pudo abrir el archivo para escritura");
	else
	{
		lockers.clear();
		manager.GetLockers(lockers);

		csvFile << "\"Locker\",\"Asignado a\",\"Fecha\",\"Usuario anterior\",\"Estado\"" << "\n";
		for (Locker& l : lockers)
		{
			csvFile << l.GetLockerNumber() << ","
				<< "\"" << l.GetAssignedUser() << "\"" << ","
				<< "\"" << l.GetDate() << "\"" << ","
				<< "\"" << l.GetPreviousUser() << "\"" << ","
				<< "\"" << l.GetStatusDescription() << "\"" << "\n";
		}
		csvFile << "\n\n\"Total lockers:\"," << manager.GetTotalLockersQty();

		std::string tmp = "Archivo: ";
		tmp.append(strFileName);
		tmp.append("\nGenerado con exito!");

		MessageBox(hWnd, tmp.data(), "Info", MB_OK);
	}

}

std::string MainWindow::GetLockersInfo()
{
	std::string strLockerInfo = "Lockers\n";
	strLockerInfo.append(std::to_string(lockers.size()));
	strLockerInfo.append(" / ");
	strLockerInfo.append(std::to_string(manager.GetTotalLockersQty()));

	return strLockerInfo;
}

void MainWindow::OnSaveFile()
{
	OPENFILENAME ofn = { 0 };
	char szFileName[255] = { 0 };

	ofn.lStructSize = sizeof(ofn);
	ofn.lpstrFile = szFileName;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_DONTADDTORECENT | OFN_NONETWORKBUTTON | OFN_OVERWRITEPROMPT;
	ofn.lpstrFilter = "Comma Separated Values (*.csv)\0.csv\0Todos los archivos\0.*\0";
	ofn.nFilterIndex = 1;
	ofn.nMaxFile = 255;
	ofn.lpstrDefExt = ".csv";
	ofn.lpstrTitle = (LPSTR)"Exportar a *.csv";
	ofn.lpstrInitialDir = (LPSTR)"%homepath%";
	ofn.hwndOwner = hWnd;

	//MessageBox(hWnd, ofn.lpstrFile, ofn.lpstrDefExt, MB_OK);

	if (GetSaveFileName(&ofn) == TRUE)
	{
		std::string fileName = ofn.lpstrFile;
		ExportCSV(fileName);
		//MessageBox(hWnd, ofn.lpstrFile, ofn.lpstrDefExt, MB_OK);
	}
}

BOOL CALLBACK MainWindow::StaticInputDlgProc(HWND hWndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	MainWindow* pThis = nullptr; 

	if (uMsg == WM_INITDIALOG)
	{
		pThis = (MainWindow*)lParam;
		SetWindowLongPtr(hWndDlg, DWLP_USER, (LONG_PTR)pThis);
	}
	else
		pThis = (MainWindow*)GetWindowLongPtr(hWndDlg, DWLP_USER);

	if (pThis)
		return pThis->InputDlgProc(hWndDlg, uMsg, wParam, lParam);

	return FALSE;
}

BOOL CALLBACK MainWindow::StaticAboutDlgProc(HWND hWndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_COMMAND:
		if (LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hWndDlg, LOWORD(wParam));
			return TRUE;
		}
	}
	
	return FALSE;
}

BOOL MainWindow::InputDlgProc(HWND hWndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
		if( btnAsignar.WasPressed() )
		{
			SetDlgItemText(hWndDlg, IDD_STATIC, "Asignar a:");
		}
		else if (btnBuscar.WasPressed())
		{
			SetDlgItemText(hWndDlg, IDD_STATIC, "Buscar a:");
		}
	break;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDD_OK:
		{
			std::fill(szDialogInput, szDialogInput + sizeof(szDialogInput), 0);

			unsigned int rtrn = GetDlgItemText(hWndDlg, IDD_TEXTBOX, szDialogInput, nMaxLoadString);
			
		}
		case IDD_CANCEL:
			EndDialog(hWndDlg, LOWORD(wParam));
			return TRUE;
		}
		break;
	}

	return FALSE;
}