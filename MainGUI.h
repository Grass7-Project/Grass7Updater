#pragma once
#pragma once

class MainGUI {
public:
	static int Init(HWND &hWnd, HINSTANCE &hInstance, LPTSTR &lpCmdLine);

private:
	static		LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};