#include "stdafx.h"
#include "ExitHandler.h"
#include "GUIDraw.h"

void ExitHandler::Cleanup()
{
	if (MainObjects.m_pITaskBarList3) {
		MainObjects.m_pITaskBarList3->Release();
	}
	CoUninitialize();
}

void ExitHandler::ErrorMessage(std::wstring &ErrorString)
{
	MainObjects.m_pITaskBarList3->SetProgressState(MainObjects.hWndMainWindow, TBPF_ERROR);
	TaskDialog(MainObjects.hWndMainWindow, nullptr, AppResStringsObjects.OSName.c_str(), AppResStringsObjects.UpdaterError.c_str(), ErrorString.c_str(), TDCBF_CLOSE_BUTTON, TD_ERROR_ICON, nullptr);
	exit(1);
}