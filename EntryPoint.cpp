#include "stdafx.h"
#include "MainInit.h"

GlobalMain MainObjects;
GlobalAppResStrings AppResStringsObjects;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);

	MSG msg;

	int ret = MainInit::Init(msg, hInstance, hPrevInstance, lpCmdLine, nCmdShow);
	if (!ret) {
		return FALSE;
	}

	return (int)msg.wParam;
}