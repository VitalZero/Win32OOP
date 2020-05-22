#pragma once
#include "BaseWindow.h"
#include <ObjIdl.h>
#include <gdiplus.h>
#include "lm/LockerManager.h"
#pragma comment(lib, "gdiplus.lib")

class ReportWindow : public BaseWindow<ReportWindow>
{
public:
	ReportWindow(LockerManager& lm_in) 
		: lm(lm_in)
	{
	}
	~ReportWindow() {}
	LPCSTR ClassName() const override { return "AdminLockerRpt"; }
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) override;

private:
	LockerManager& lm;
	std::vector<Locker> lockers;
	void Print();
	void Paint();
};