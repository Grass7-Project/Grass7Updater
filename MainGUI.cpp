#include "stdafx.h"
#include "MainGUI.h"
#include "GUIDraw.h"
#include "MainCode.h"

MainGUI MainGUIObjects;

int MainGUI::Init()
{
	if (FAILED(CoCreateInstance(CLSID_TaskbarList, nullptr, CLSCTX_INPROC_SERVER, IID_ITaskbarList3, (void **)&MainObjects.m_pITaskBarList3))) {
		return 1;
	}

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
	wcex.hCursor = LoadCursorW(MainObjects.hInst, IDC_ARROW);
	wcex.lpfnWndProc = WndProc;
	wcex.hInstance = MainObjects.hInst;

	if (!RegisterClassExW(&wcex))
		return 1;

	MainObjects.hWndMainWindow = ::CreateWindowExW(
		0,
		L"GRASS7UPDATER",
		AppResStringsObjects.OSName.c_str(),
		WS_OVERLAPPED | WS_CAPTION,
		0,
		0,
		386,
		155,
		nullptr,
		nullptr,
		MainObjects.hInst,
		nullptr);

	if (!MainObjects.hWndMainWindow)
		return 1;

	MainGUIObjects.hSmoothProgressCtrl = ::CreateWindowExW(
		0,
		PROGRESS_CLASS,
		L"",
		WS_CHILD | WS_VISIBLE | PBS_SMOOTH,
		10,
		40,
		346,
		20,
		MainObjects.hWndMainWindow,
		(HMENU)ID_SMOOTHPROGRESSCTRL,
		MainObjects.hInst,
		nullptr);

	if (!MainGUIObjects.hSmoothProgressCtrl)
		return 1;

	MainObjects.ExtractionPercentage = 0;

	GUIDraw::CreateQuestion();

	ShowWindow(MainObjects.hWndMainWindow, SW_SHOWDEFAULT);
	UpdateWindow(MainObjects.hWndMainWindow);

	std::thread MainCodeThread(MainCode::mainCode, std::ref(MainObjects.CommandLine));
	MainCodeThread.detach();

	MSG msg;
	while (GetMessageW(&msg, nullptr, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessageW(&msg);
	}

	::UnregisterClassW(wcex.lpszClassName, MainObjects.hInst);

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
		GUIDraw::updateWindow(MainObjects.ExtractionPercentage, MainGUIObjects.hSmoothProgressCtrl, MainObjects.hWndMainWindow, hdc, AppResStringsObjects.Extracting);
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