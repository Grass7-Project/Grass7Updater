#include "stdafx.h"
#include "MainInit.h"
#include "FileManagement.h"
#include "ResourceLoader.h"
#include "MainGUI.h"

int MainInit::Init(MSG &msg, HINSTANCE &hInstance, HINSTANCE &hPrevInstance, LPTSTR &lpCmdLine, int &nCmdShow)
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

	if (MainObjects.driveletter == L"") {
		TaskDialog(NULL, NULL, AppResStringsObjects.OSName.c_str(), AppResStringsObjects.UpdaterError.c_str(), AppResStringsObjects.NotInstalled.c_str(), TDCBF_CLOSE_BUTTON, TD_ERROR_ICON, NULL);
		exit(0);
	}

	// Set folder paths
	MainObjects.updatefld = MainObjects.driveletter.c_str();
	MainObjects.updatefld.append(L"gr7updatefld");

	MainObjects.system32fld = MainObjects.driveletter;
	MainObjects.system32fld.append(L"Windows\\System32");

	if (Grass7API::FileManagement::dirExists(MainObjects.updatefld.c_str()) == 1) {
		TaskDialog(NULL, NULL, AppResStringsObjects.OSName.c_str(), AppResStringsObjects.UpdaterError.c_str(), AppResStringsObjects.UpdateInProgress.c_str(), TDCBF_CLOSE_BUTTON, TD_ERROR_ICON, NULL);
		exit(0);
	}

	if (!MainGUI::Init(MainObjects.hWndMainWindow, MainObjects.hInst, lpCmdLine)) {
		return FALSE;
	}

	return 0;
}