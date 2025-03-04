#include "stdafx.h"
#include "MainInit.h"
#include "FileManagement.h"
#include "ResourceLoader.h"
#include "MainGUI.h"
#include "ExitHandler.h"

int MainInit::Init(HINSTANCE &hInstance, LPTSTR &lpCmdLine)
{
	MainObjects.hInst = hInstance;
	MainObjects.CommandLine.append(lpCmdLine);
	std::atexit(ExitHandler::Cleanup);

	if (FAILED(CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED))) {
		return 1;
	}

	ResourceLoader::LoadStrings();

	std::wstring driveletterWstr(MAX_PATH, 0);
	Grass7API::FileManagement::GetSystemDriveLetter(&driveletterWstr[0]);
	MainObjects.driveletter.append(&driveletterWstr[0]);

	if (!SUCCEEDED(Grass7API::Privilege::ModifyPrivilege(SE_RESTORE_NAME, TRUE, GetCurrentProcess()))) {
		TaskDialog(nullptr, nullptr, AppResStringsObjects.OSName.c_str(), AppResStringsObjects.UpdaterError.c_str(), AppResStringsObjects.PrivilageError.c_str(), TDCBF_CLOSE_BUTTON, TD_ERROR_ICON, nullptr);
		return 1;
	}

	if (!SUCCEEDED(Grass7API::Privilege::ModifyPrivilege(SE_BACKUP_NAME, TRUE, GetCurrentProcess()))) {
		TaskDialog(nullptr, nullptr, AppResStringsObjects.OSName.c_str(), AppResStringsObjects.UpdaterError.c_str(), AppResStringsObjects.PrivilageError.c_str(), TDCBF_CLOSE_BUTTON, TD_ERROR_ICON, nullptr);
		return 1;
	}

	if (MainObjects.driveletter.empty()) {
		TaskDialog(nullptr, nullptr, AppResStringsObjects.OSName.c_str(), AppResStringsObjects.UpdaterError.c_str(), AppResStringsObjects.NotInstalled.c_str(), TDCBF_CLOSE_BUTTON, TD_ERROR_ICON, nullptr);
		return 1;
	}

	MainObjects.updatefld.append(MainObjects.driveletter);
	MainObjects.updatefld.append(L"gr7updatefld");

	MainObjects.system32fld.append(MainObjects.driveletter);
	MainObjects.system32fld.append(L"Windows\\System32");

	if (Grass7API::FileManagement::dirExists(MainObjects.updatefld.c_str()) == 1) {
		TaskDialog(nullptr, nullptr, AppResStringsObjects.OSName.c_str(), AppResStringsObjects.UpdaterError.c_str(), AppResStringsObjects.UpdateInProgress.c_str(), TDCBF_CLOSE_BUTTON, TD_ERROR_ICON, nullptr);
		return 1;
	}

	if (!MainGUI::Init()) {
		return FALSE;
	}

	return 0;
}