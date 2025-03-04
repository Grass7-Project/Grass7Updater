#pragma once

class MainGUI {
public:
	static int Init();
private:
	static		LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	HWND hSmoothProgressCtrl;
};