#include "stdafx.h"
#include "MainGUI.h"
#include "GUIDraw.h"
#include "MainCode.h"

TCHAR wcs[MAX_PATH];

int MainGUI::Init(HWND &hWnd, HINSTANCE &hInstance, LPTSTR &lpCmdLine)
{
	INITCOMMONCONTROLSEX iccex;
	iccex.dwSize = sizeof(iccex);
	iccex.dwICC = ICC_PROGRESS_CLASS;

	HBRUSH hb = ::CreateSolidBrush(RGB(255, 255, 255));

	if (!InitCommonControlsEx(&iccex))
		return 1;

	WNDCLASSEX wcex;
	ZeroMemory(&wcex, sizeof(wcex));

	wcex.cbSize = sizeof(wcex);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpszClassName = L"GRASS7UPDATER";
	wcex.hbrBackground = hb;
	wcex.hCursor = LoadCursorW(hInstance, IDC_ARROW);
	wcex.lpfnWndProc = WndProc;
	wcex.hInstance = hInstance;

	if (!RegisterClassExW(&wcex))
		return 1;

	int x = GetSystemMetrics(SM_CXSCREEN);
	int y = GetSystemMetrics(SM_CYSCREEN);

	hWnd = ::CreateWindowExW(
		0,
		L"GRASS7UPDATER",
		AppResStringsObjects.OSName.c_str(),
		WS_OVERLAPPED | WS_CAPTION,
		0,
		0,
		386,
		155,
		NULL,
		NULL,
		hInstance,
		NULL);

	if (!hWnd)
		return 1;

	HWND hSmoothProgressCtrl;

	hSmoothProgressCtrl = ::CreateWindowExW(
		0,
		PROGRESS_CLASS,
		L"",
		WS_CHILD | WS_VISIBLE | PBS_SMOOTH,
		10,
		40,
		346,
		20,
		hWnd,
		(HMENU)ID_SMOOTHPROGRESSCTRL,
		hInstance,
		NULL);

	wcsncat_s(wcs, AppResStringsObjects.Installing.c_str(), 256);
	wcsncat_s(wcs, L"0%", 256);
	::SendMessageW(hSmoothProgressCtrl, PBM_SETPOS, (WPARAM)(INT)0, 0);
	std::thread MainCodeThread(MainCodeClass::mainCode, std::ref(hSmoothProgressCtrl), std::ref(hWnd), std::ref(wcs), std::ref(lpCmdLine));
	MainCodeThread.detach();

	MSG msg;
	while (::GetMessageW(&msg, hWnd, 0, 0) > 0)
		::DispatchMessageW(&msg);

	::UnregisterClassW(wcex.lpszClassName, hInstance);

	return (int)msg.wParam;
}

LRESULT CALLBACK MainGUI::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		GUIDraw::DrawValues(hdc, wcs);
		EndPaint(hWnd, &ps);
	}
	break;

	case WM_DESTROY:
		::PostQuitMessage(0);
		break;

	default:
		return ::DefWindowProcW(hWnd, uMsg, wParam, lParam);
	}

	return 0;
}