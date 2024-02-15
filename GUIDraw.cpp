// GUI Draw code

#include "stdafx.h"
#include "GUIDraw.h"
#include "FileManagement.h"
#include "Global.h"

void GUIDraw::OnSomeActionToRefreshValues(HWND hWnd, wchar_t *wcs)
{
    HDC hdc = ::GetDC(hWnd);
    DrawValues(hdc, wcs);
    ReleaseDC(hWnd,hdc);
}

void GUIDraw::DrawValues(HDC hdc, wchar_t *wcs)
{
	HFONT hFont,hTmp;
	hFont=CreateFont(18,0,0,0,FW_LIGHT,0,0,0,0,0,0,2,0,L"Segoe UI");
	hTmp=(HFONT)SelectObject(hdc,hFont);
	size_t size = wcslen(wcs);
	int convertsize = static_cast<int>(size);
    TextOut(hdc, 10, 10, wcs, convertsize);
}

void GUIDraw::updateProgressBar(int percentageCounter, HWND hProgressBar, HWND hWnd,wchar_t *hProgressText)
{
	TCHAR percantage[5] = { 0 };
	TCHAR ProgressText[24] = { 0 };
	
	memset(hProgressText, 0, sizeof(hProgressText));
	swprintf_s(percantage, L"%d", percentageCounter);
	wcsncat_s(percantage, L"%", 5);
	::SendMessage(hProgressBar, PBM_SETPOS, (WPARAM)(INT)percentageCounter, 0);
	wcscpy_s(ProgressText, hProgressText);
	wcsncat_s(ProgressText, AppResStringsObjects.Installing.c_str(), 24);
	wcsncat_s(ProgressText, percantage, 256);
	OnSomeActionToRefreshValues(hWnd, ProgressText);
	::UpdateWindow(hWnd);
}

void GUIDraw::CreateQuestion()
{
	std::wstring updatefolder = MainObjects.driveletter;
	updatefolder.append(L"gr7updatefld");
	std::wstring sys32dir = MainObjects.driveletter;
	sys32dir.append(L"Windows\\System32");

	int respond = 0;
	TaskDialog(NULL, NULL, AppResStringsObjects.OSName.c_str(), AppResStringsObjects.Question.c_str(), NULL, TDCBF_YES_BUTTON | TDCBF_NO_BUTTON, TD_INFORMATION_ICON, &respond);
	if(respond != IDYES) {
		SetCurrentDirectoryW(sys32dir.c_str());
		gr7::DeleteDirectory(updatefolder.c_str());
		exit(0);
	}
}