#include "stdafx.h"
#include "FileManagement.h"
#include "MainCode.h"
#include "GUIDraw.h"
#include "ConfigManager.h"
#include "ExitHandler.h"

MainCode MainCodeObjects;

void MainCode::mainCode(std::wstring &UpdateFile)
{
	UpdateFile.erase(std::remove(UpdateFile.begin(), UpdateFile.end(), L'"'), UpdateFile.end());

	CreateDirectoryW(MainObjects.updatefld.c_str(), nullptr);
	SetCurrentDirectoryW(MainObjects.updatefld.c_str());

	if (FileManagement::extract(UpdateFile)) {
		SetCurrentDirectoryW(MainObjects.system32fld.c_str());
		Grass7API::FileManagement::DeleteDirectory(MainObjects.updatefld.c_str());
		ExitHandler::ErrorMessage(AppResStringsObjects.ArchiveFail);
	}
	SetCurrentDirectoryW(MainObjects.system32fld.c_str());

	MainCodeObjects.UpdateConfigFile.append(MainObjects.driveletter);
	MainCodeObjects.UpdateConfigFile.append(L"gr7updatefld\\Update.conf");
	if (ConfigManager::LoadConfig(MainCodeObjects.UpdateConfigFile)) {
		ExitHandler::ErrorMessage(AppResStringsObjects.ConfigAccessError);
	}

#if defined(_M_X64)
	std::wstring CompiledArch = L"amd64";
#endif
#if defined(_M_IX86)
	std::wstring CompiledArch = L"i386";
#endif

	if (ConfigObjects.Architecture.compare(CompiledArch)) {
		Grass7API::FileManagement::DeleteDirectory(MainObjects.updatefld.c_str());
		ExitHandler::ErrorMessage(AppResStringsObjects.ArchNotMatch);
	}

	HKEY UpdateRegistryKey;
	HKEY hKeyGr7CurrentVersion;

	MainCodeObjects.UpdateIDRegistry.append(L"Software\\Grass7\\CurrentVersion\\Updates\\" + ConfigObjects.UpdateID);
	if (RegOpenKeyExW(HKEY_LOCAL_MACHINE, MainCodeObjects.UpdateIDRegistry.c_str(), 0, KEY_READ, &UpdateRegistryKey) == ERROR_SUCCESS)
	{
		Grass7API::FileManagement::DeleteDirectory(MainObjects.updatefld.c_str());
		ExitHandler::ErrorMessage(AppResStringsObjects.AlreadyInstalled);
	}
	if (RegOpenKeyExW(HKEY_LOCAL_MACHINE, L"Software\\Grass7\\CurrentVersion", 0, KEY_READ, &hKeyGr7CurrentVersion) != ERROR_SUCCESS)
	{
		Grass7API::FileManagement::DeleteDirectory(MainObjects.updatefld.c_str());
		ExitHandler::ErrorMessage(AppResStringsObjects.FailOpenKey);
	}

	std::wstring UpdateLevel(MAX_PATH, 0);
	DWORD dwBufferSize = (DWORD)UpdateLevel.size();
	RegQueryValueExW(hKeyGr7CurrentVersion, L"UpdateLevel", 0, nullptr, (LPBYTE)&UpdateLevel[0], &dwBufferSize);

	std::wstring PlatformID = ConfigObjects.UpdateID.substr(0, ConfigObjects.UpdateID.find(L"-"));
	std::wstring PlatformIDMatch = L"GR7U";
	PlatformIDMatch.append(&UpdateLevel[0]);

	if (PlatformID.compare(PlatformIDMatch)) {
		Grass7API::FileManagement::DeleteDirectory(MainObjects.updatefld.c_str());
		ExitHandler::ErrorMessage(AppResStringsObjects.FailOpenKey);
	}

	if (ConfigObjects.RequiredUpdate.compare(L"none")) {
		MainCodeObjects.RequiredUpdateRegistry.append(L"Software\\Grass7\\CurrentVersion\\Updates\\" + ConfigObjects.RequiredUpdate);
		if (RegOpenKeyExW(HKEY_LOCAL_MACHINE, MainCodeObjects.RequiredUpdateRegistry.c_str(), 0, KEY_READ, &UpdateRegistryKey) != ERROR_SUCCESS)
		{
			std::wstring ErrorString = AppResStringsObjects.UpdateReq1;
			ErrorString.append(ConfigObjects.RequiredUpdate + AppResStringsObjects.UpdateReq2);

			Grass7API::FileManagement::DeleteDirectory(MainObjects.updatefld.c_str());
			ExitHandler::ErrorMessage(ErrorString);
		}
	}

	SetCurrentDirectoryW(MainObjects.updatefld.c_str());

	if (ConfigObjects.OSCommands == 1) {
		std::wstring cmddlol = MainObjects.driveletter;
		cmddlol.append(L"gr7updatefld\\OScommands.bat");

		SHELLEXECUTEINFO ShExecInfo;
		ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
		ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
		ShExecInfo.hwnd = nullptr;
		ShExecInfo.lpVerb = L"open";
		ShExecInfo.lpFile = cmddlol.c_str();
		ShExecInfo.lpParameters = L"";
		ShExecInfo.lpDirectory = MainObjects.system32fld.c_str();
		ShExecInfo.nShow = SW_HIDE;
		ShExecInfo.hInstApp = nullptr;
		ShellExecuteExW(&ShExecInfo);
		WaitForSingleObject(ShExecInfo.hProcess, INFINITE);
		CloseHandle(ShExecInfo.hProcess);
	}

	// Reboot to winRE
	SHELLEXECUTEINFO ShExecInfo2;
	ShExecInfo2.cbSize = sizeof(SHELLEXECUTEINFO);
	ShExecInfo2.fMask = NULL;
	ShExecInfo2.hwnd = nullptr;
	ShExecInfo2.lpVerb = L"runas";
	ShExecInfo2.lpFile = L"reagentc";
	ShExecInfo2.lpParameters = L"/boottore";
	ShExecInfo2.lpDirectory = MainObjects.system32fld.c_str();
	ShExecInfo2.nShow = SW_HIDE;
	ShExecInfo2.hInstApp = nullptr;

	ShellExecuteExW(&ShExecInfo2);
	WaitForSingleObject(ShExecInfo2.hProcess, INFINITE);
	CloseHandle(ShExecInfo2.hProcess);
	RestartDialogEx(MainObjects.hWndMainWindow, L"", EWX_REBOOT, NULL);
	exit(0);
}