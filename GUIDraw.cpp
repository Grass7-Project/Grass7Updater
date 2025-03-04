#include "stdafx.h"
#include "GUIDraw.h"
#include "FileManagement.h"

GUIDraw GUIDrawObjects;

void GUIDrawUpdateWindow(int percentageCounter, HWND &hProgressBar, HWND &hWnd, HDC &hdc, std::wstring &hProgressText)
{
	RECT rchWnd;
	std::wstring percentage(MAX_PATH, 0);
	std::wstring ProgressText;

	percentage.resize((size_t)swprintf_s(&percentage[0], percentage.size(), L"%d", percentageCounter));
	ProgressText.append(hProgressText + &percentage[0] + L"%");
	SendMessageW(hProgressBar, PBM_SETPOS, (WPARAM)(INT)percentageCounter, 0);
	GetClientRect(hWnd, &rchWnd);
	Grass7API::Paint::PaintText(hdc, 10, 10, L"Segoe UI", RGB(0, 0, 0), ProgressText.c_str(), 18, 2, OPAQUE, FW_LIGHT, &rchWnd);
}

void GUIDraw::CreateQuestion()
{
	int respond = 0;
	TaskDialog(nullptr, nullptr, AppResStringsObjects.OSName.c_str(), AppResStringsObjects.Question.c_str(), nullptr, TDCBF_YES_BUTTON | TDCBF_NO_BUTTON, TD_INFORMATION_ICON, &respond);
	if(respond != IDYES) {
		exit(0);
	}
}