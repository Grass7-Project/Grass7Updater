#include "stdafx.h"
#include "FileManagement.h"
#include "MainCode.h"
#include "GUIDraw.h"
#include <sdkddkver.h>
#include <vector>

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

TCHAR wcs[256];

LRESULT
CALLBACK
WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_PAINT:
			{
				PAINTSTRUCT ps;
				HDC hdc = BeginPaint(hWnd, &ps);
				DrawValues(hdc, wcs);
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

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nShowCmd)
{
    if (!SUCCEEDED(gr7::ModifyPrivilege(SE_RESTORE_NAME, TRUE, GetCurrentProcess()))) {
		TaskDialog(NULL, NULL, gr7::LoadStringToW(GetModuleHandleW(NULL),IDS_OSNAME),  gr7::LoadStringToW(GetModuleHandleW(NULL),IDS_UPDATER_ERROR), gr7::LoadStringToW(GetModuleHandleW(NULL),IDS_PRIVILAGE_ERROR), TDCBF_CLOSE_BUTTON, TD_ERROR_ICON, NULL);
		exit(0);
	}

    if (!SUCCEEDED(gr7::ModifyPrivilege(SE_BACKUP_NAME, TRUE, GetCurrentProcess()))) {
		TaskDialog(NULL, NULL, gr7::LoadStringToW(GetModuleHandleW(NULL),IDS_OSNAME),  gr7::LoadStringToW(GetModuleHandleW(NULL),IDS_UPDATER_ERROR), gr7::LoadStringToW(GetModuleHandleW(NULL),IDS_PRIVILAGE_ERROR), TDCBF_CLOSE_BUTTON, TD_ERROR_ICON, NULL);
		exit(0);
	}

	if(gr7::GetSystemDriveLetter() == "") {
		TaskDialog(NULL, NULL, gr7::LoadStringToW(GetModuleHandleW(NULL),IDS_OSNAME),  gr7::LoadStringToW(GetModuleHandleW(NULL),IDS_UPDATER_ERROR), gr7::LoadStringToW(GetModuleHandleW(NULL),IDS_NOT_INSTALLED), TDCBF_CLOSE_BUTTON, TD_ERROR_ICON, NULL);
		exit(0);
	}
	char bufferp[256] = { 0 };
	strncpy_s(bufferp, gr7::GetSystemDriveLetter(), sizeof(bufferp));
	strncat_s(bufferp, "gr7updatefld", sizeof(bufferp));

	if(gr7::dirExists(bufferp) == 1) {
		TaskDialog(NULL, NULL, gr7::LoadStringToW(GetModuleHandleW(NULL),IDS_OSNAME),  gr7::LoadStringToW(GetModuleHandleW(NULL),IDS_UPDATER_ERROR), gr7::LoadStringToW(GetModuleHandleW(NULL),IDS_UPDATE_IN_PROGRESS), TDCBF_CLOSE_BUTTON, TD_ERROR_ICON, NULL);
		memset(bufferp, 0, sizeof(bufferp));
		exit(0);
	}
	memset(bufferp, 0, sizeof(bufferp));

	INITCOMMONCONTROLSEX iccex;
	iccex.dwSize = sizeof(iccex);
	iccex.dwICC = ICC_PROGRESS_CLASS;

	HBRUSH hb = ::CreateSolidBrush(RGB(255,255,255));

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

	HWND hWnd = ::CreateWindowExW(
		0,
		L"GRASS7UPDATER",
		gr7::LoadStringToW(GetModuleHandleW(NULL),IDS_OSNAME),
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

	wcsncat_s(wcs, gr7::LoadStringToW(GetModuleHandleW(NULL),IDS_INSTALLING), 256);
	wcsncat_s(wcs, L"0%", 256);
	::SendMessageW(hSmoothProgressCtrl, PBM_SETPOS, (WPARAM)(INT)0, 0);
	mainCode(hSmoothProgressCtrl, hWnd, wcs, lpCmdLine);

	MSG msg;
	while (::GetMessageW(&msg, hWnd, 0, 0) > 0)
		::DispatchMessageW(&msg);

	::UnregisterClassW(wcex.lpszClassName, hInstance);

	return (int)msg.wParam;
}