#include "stdafx.h"
#include "FileManagement.h"
#include "MainCode.h"
#include "GUIDraw.h"
#include "ResourceLoader.h"
#include "Global.h"
#include <sdkddkver.h>
#include <vector>
#include <thread>

GlobalMain MainObjects;
GlobalAppResStrings AppResStringsObjects;

TCHAR wcs[MAX_PATH];

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

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nShowCmd)
{
	MainObjects.hInst = hInstance;
	ResourceLoader::LoadStrings();

	std::wstring driveletterWstr(MAX_PATH, 0);
	Grass7API::FileManagement::GetSystemDriveLetter(&driveletterWstr[0]);
	MainObjects.driveletter = driveletterWstr.c_str();

    if (!SUCCEEDED(Grass7API::Privilege::ModifyPrivilege(SE_RESTORE_NAME, TRUE, GetCurrentProcess()))) {
		TaskDialog(NULL, NULL, AppResStringsObjects.OSName.c_str(), AppResStringsObjects.UpdaterError.c_str(), AppResStringsObjects.PrivilageError.c_str(), TDCBF_CLOSE_BUTTON, TD_ERROR_ICON, NULL);
		exit(0);
	}

    if (!SUCCEEDED(Grass7API::Privilege::ModifyPrivilege(SE_BACKUP_NAME, TRUE, GetCurrentProcess()))) {
		TaskDialog(NULL, NULL, AppResStringsObjects.OSName.c_str(), AppResStringsObjects.UpdaterError.c_str(), AppResStringsObjects.PrivilageError.c_str(), TDCBF_CLOSE_BUTTON, TD_ERROR_ICON, NULL);
		exit(0);
	}

	if(MainObjects.driveletter == L"") {
		TaskDialog(NULL, NULL, AppResStringsObjects.OSName.c_str(), AppResStringsObjects.UpdaterError.c_str(), AppResStringsObjects.NotInstalled.c_str(), TDCBF_CLOSE_BUTTON, TD_ERROR_ICON, NULL);
		exit(0);
	}

	// Set folder paths
	MainObjects.updatefld = MainObjects.driveletter.c_str();
	MainObjects.updatefld.append(L"gr7updatefld");

	MainObjects.system32fld = MainObjects.driveletter;
	MainObjects.system32fld.append(L"Windows\\System32");

	if(Grass7API::FileManagement::dirExists(MainObjects.updatefld.c_str()) == 1) {
		TaskDialog(NULL, NULL, AppResStringsObjects.OSName.c_str(), AppResStringsObjects.UpdaterError.c_str(), AppResStringsObjects.UpdateInProgress.c_str(), TDCBF_CLOSE_BUTTON, TD_ERROR_ICON, NULL);
		exit(0);
	}

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