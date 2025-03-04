#pragma once

class GUIDraw {
public:
	static void updateWindow(int percentageCounter, HWND &hProgressBar, HWND &hWnd, HDC &hdc, std::wstring &hProgressText);
	static void CreateQuestion();
};