#pragma once
#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#include "BaseWindow.h"
#include "ReportWindow.h"
#include "lm/LockerManager.h"
#include "controls/button.h"
#include "controls/ListView.h"
#include "controls/Static.h"

class MainWindow : public BaseWindow<MainWindow>
{
public:
	MainWindow();
	~MainWindow() {}

public:
	LPCSTR ClassName() const override { return szClassName; }
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) override;
	BOOL InputDlgProc(HWND hWndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
	static BOOL CALLBACK SetChildWndFontProc(HWND hWndChild, LPARAM font);
	static BOOL CALLBACK StaticInputDlgProc(HWND hWndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
	static BOOL CALLBACK StaticAboutDlgProc(HWND hWndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	void OnCreate();
	void OnCommand(WORD wCmdId);
	void OnNotify(LPARAM lParam);
	void OnSaveFile();
	void ExportCSV(std::string& strFileName);
	std::string GetLockersInfo();

private:
	enum CONTROLS_ID { ID_LISTVIEW_MAIN = 1200, ID_BUTTON_ASIGNAR, ID_BUTTON_VERTODOS, ID_BUTTON_BUSCAR,
		ID_BUTTON_ELIMINAR, ID_BUTTON_AGREGAR, ID_MENU_VERASIGNADOS, ID_MENU_VERSINASIGNAR, ID_STATIC_LOCKERS };
	ListView lvMain;
	Button btnAsignar;
	Button btnFiltro;
	Button btnBuscar;
	Button btnEliminar;
	Button btnAgregar;
	Static lblLockers;
	char szDialogInput[nMaxLoadString] = {0};
	LockerManager manager;
	std::vector<Locker> lockers;
	HIMAGELIST hImage;
	HFONT hFont = nullptr;
	ReportWindow rpt;
};