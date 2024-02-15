// The code to open and extract the update package

#include "stdafx.h"
#include "FileManagement.h"
#include "MainCode.h"
#include "GUIDraw.h"
#include "Global.h"
#include <algorithm>
#include <shlobj.h>
#include <regex>
#include <vector>
#include <filesystem>

int MainCodeClass::mainCode(HWND hProgressBar, HWND hWnd, wchar_t *hProgressText, const wchar_t *lpCmdLine)
{
	std::wstring driveletterD(MAX_PATH, 0);
	driveletterD.resize((size_t)gr7::GetSystemDriveLetter(&driveletterD[0]));
	MainObjects.driveletter = driveletterD;

	std::wstring lpCmdLineW = lpCmdLine;
	lpCmdLineW.erase(std::remove(lpCmdLineW.begin(), lpCmdLineW.end(), L'"'), lpCmdLineW.end());
	Sleep(2000);

	if (MainObjects.driveletter == L"") {
		TaskDialog(NULL, NULL, AppResStringsObjects.OSName.c_str(), AppResStringsObjects.UpdaterError.c_str(), AppResStringsObjects.NotInstalled.c_str(), TDCBF_CLOSE_BUTTON, TD_ERROR_ICON, NULL);
		memset(hProgressText, 0, sizeof(hProgressText));
		exit(0);
	}
	std::wstring updatefld = MainObjects.driveletter;
	updatefld.append(L"gr7updatefld");

	CreateDirectoryW(updatefld.c_str(), NULL);
	SetCurrentDirectoryW(updatefld.c_str());

	std::wstring system32fld = MainObjects.driveletter;
	system32fld.append(L"Windows\\System32");

	int archiveerr = FileManagementClass::extract(lpCmdLineW.c_str());
	if (archiveerr != 0) {
		SetCurrentDirectoryW(system32fld.c_str());
		gr7::DeleteDirectory(updatefld.c_str());
		TaskDialog(NULL, NULL, AppResStringsObjects.OSName.c_str(), AppResStringsObjects.UpdaterError.c_str(), AppResStringsObjects.ArchiveFail.c_str(), TDCBF_CLOSE_BUTTON, TD_ERROR_ICON, NULL);
		memset(hProgressText, 0, sizeof(hProgressText));
		exit(0);
	}

	// We load and parse the config file
	// We have to do a diarrhea way of doing this, not proud of this one, there is likely a better way but i was very angry at the time that it didnt work so i did this fucking shit.
	std::wstring UpdateConfigFile = MainObjects.driveletter;
	UpdateConfigFile.append(L"gr7updatefld\\Update.conf");

	std::wifstream ifs(UpdateConfigFile);
	std::wstring updateID = L"UpdateID=";
	std::wstring arch = L"arch=";
	std::wstring requiredupdate = L"requiredupdate=";
	std::wstring OScmd = L"OScommands=";
	std::wregex updateids(updateID + L"\\b");
	std::wregex archs(arch + L"\\b");
	std::wregex requiredupdates(requiredupdate + L"\\b");
	std::wregex OScmden(OScmd + L"\\b");

	std::wstring lineUpdateID;
	std::wstring lineArch;
	std::wstring lineRequiredupdate;
	std::wstring lineOScommands;

	getline(ifs, lineUpdateID);
	if (regex_search(lineUpdateID, updateids)) {
		std::size_t pos = lineUpdateID.find(L"=");
		if (pos != std::wstring::npos)
		{
			lineUpdateID = lineUpdateID.substr(pos + 1);
		}
	}
	getline(ifs, lineArch);
	if (regex_search(lineArch, archs)) {
		std::size_t pos = lineArch.find(L"=");
		if (pos != std::wstring::npos)
		{
			lineArch = lineArch.substr(pos + 1);
		}
	}
	getline(ifs, lineRequiredupdate);
	if (regex_search(lineRequiredupdate, requiredupdates)) {
		std::size_t pos = lineRequiredupdate.find(L"=");
		if (pos != std::wstring::npos)
		{
			lineRequiredupdate = lineRequiredupdate.substr(pos + 1);
		}
	}
	int enableOScommands = 0;
	getline(ifs, lineOScommands);
	if (regex_search(lineOScommands, OScmden)) {
		if (lineOScommands.find(L"true") != std::wstring::npos) {
			enableOScommands = 1;
		}
	}
	ifs.close();

	HKEY hkey1;
	HKEY hkey1d;

	SetCurrentDirectoryW(system32fld.c_str());

	std::wstring platformID1 = lineUpdateID.substr(0, lineUpdateID.find(L"-"));

	std::wstring UpdateIDReg = L"Software\\Grass7\\CurrentVersion\\Updates\\";
	UpdateIDReg.append(lineUpdateID);

	std::wstring RequiredUpdateReg = L"Software\\Grass7\\CurrentVersion\\Updates\\";
	RequiredUpdateReg.append(lineRequiredupdate);

	std::wstring lineRequiredupdateW = std::wstring(lineRequiredupdate.begin(), lineRequiredupdate.end());
	std::wstring platformID1W = std::wstring(platformID1.begin(), platformID1.end());

#if defined(_M_X64)
	const wchar_t *specificarch = L"amd64";
#endif
#if defined(_M_IX86)
	const wchar_t *specificarch = L"i386";
#endif

	if (lineArch != specificarch) {
		TaskDialog(NULL, NULL, AppResStringsObjects.OSName.c_str(), AppResStringsObjects.UpdaterError.c_str(), AppResStringsObjects.ArchNotMatch.c_str(), TDCBF_CLOSE_BUTTON, TD_ERROR_ICON, NULL);
		gr7::DeleteDirectory(updatefld.c_str());
		memset(hProgressText, 0, sizeof(hProgressText));
		exit(0);
	}

	// Registry stuff
	LONG lResult = RegOpenKeyExW(HKEY_LOCAL_MACHINE, UpdateIDReg.c_str(), 0, KEY_READ, &hkey1);
	if (lResult == ERROR_SUCCESS)
	{
		TaskDialog(NULL, NULL, AppResStringsObjects.OSName.c_str(), AppResStringsObjects.UpdaterError.c_str(), AppResStringsObjects.AlreadyInstalled.c_str(), TDCBF_CLOSE_BUTTON, TD_ERROR_ICON, NULL);
		gr7::DeleteDirectory(updatefld.c_str());
		memset(hProgressText, 0, sizeof(hProgressText));
		lineRequiredupdate.clear();
		lineRequiredupdateW.clear();
		platformID1.clear();
		platformID1W.clear();
		exit(0);
	}
	LONG lResult1 = RegOpenKeyExW(HKEY_LOCAL_MACHINE, L"Software\\Grass7\\CurrentVersion", 0, KEY_READ, &hkey1d);
	if (lResult1 != ERROR_SUCCESS)
	{
		TaskDialog(NULL, NULL, AppResStringsObjects.OSName.c_str(), AppResStringsObjects.UpdaterError.c_str(), AppResStringsObjects.FailOpenKey.c_str(), TDCBF_CLOSE_BUTTON, TD_ERROR_ICON, NULL);
		gr7::DeleteDirectory(updatefld.c_str());
		memset(hProgressText, 0, sizeof(hProgressText));
		exit(0);
	}

	WCHAR szBuffer[512];
	DWORD dwBufferSize = sizeof(szBuffer);
	ULONG nError;
	nError = RegQueryValueExW(hkey1d, L"UpdateLevel", 0, NULL, (LPBYTE)szBuffer, &dwBufferSize);

	std::wstring szBuffer1WS = L"GR7U";
	szBuffer1WS.append(szBuffer);

	if (platformID1W.find(szBuffer1WS) == std::wstring::npos) {
		TaskDialog(NULL, NULL, AppResStringsObjects.OSName.c_str(), AppResStringsObjects.UpdaterError.c_str(), AppResStringsObjects.NotForThis.c_str(), TDCBF_CLOSE_BUTTON, TD_ERROR_ICON, NULL);
		gr7::DeleteDirectory(updatefld.c_str());
		memset(hProgressText, 0, sizeof(hProgressText));
		exit(0);
	}

	if (lineRequiredupdate.find(L"none") == std::wstring::npos) {
		LONG lResult = RegOpenKeyExW(HKEY_LOCAL_MACHINE, RequiredUpdateReg.c_str(), 0, KEY_READ, &hkey1);
		if (lResult != ERROR_SUCCESS)
		{
			std::wstring ErrorString = AppResStringsObjects.UpdateReq1;
			ErrorString.append(lineRequiredupdateW);
			ErrorString.append(AppResStringsObjects.UpdateReq2);

			TaskDialog(NULL, NULL, AppResStringsObjects.OSName.c_str(), AppResStringsObjects.UpdaterError.c_str(), ErrorString.c_str(), TDCBF_CLOSE_BUTTON, TD_ERROR_ICON, NULL);
			gr7::DeleteDirectory(updatefld.c_str());
			memset(hProgressText, 0, sizeof(hProgressText));
			exit(0);
		}
	}

	lineRequiredupdate.clear();
	lineRequiredupdateW.clear();
	platformID1.clear();
	platformID1W.clear();
	GUIDraw::CreateQuestion();
	::ShowWindow(hWnd, SW_SHOWDEFAULT);
	::UpdateWindow(hWnd);
	int percentageCounter = 0;
	GUIDraw::updateProgressBar(percentageCounter, hProgressBar, hWnd, hProgressText);

	SetCurrentDirectoryW(updatefld.c_str());

	if (enableOScommands == 1) {
		std::wstring cmddlol = MainObjects.driveletter;
		cmddlol.append(L"gr7updatefld\\OScommands.bat");

		SHELLEXECUTEINFO ShExecInfo;
		ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
		ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
		ShExecInfo.hwnd = NULL;
		ShExecInfo.lpVerb = L"open";
		ShExecInfo.lpFile = cmddlol.c_str();
		ShExecInfo.lpParameters = L"";
		ShExecInfo.lpDirectory = system32fld.c_str();
		ShExecInfo.nShow = SW_HIDE;
		ShExecInfo.hInstApp = NULL;
		ShellExecuteExW(&ShExecInfo);
		WaitForSingleObject(ShExecInfo.hProcess, INFINITE);
		CloseHandle(ShExecInfo.hProcess);
	}

	// We reboot to winRE
	SHELLEXECUTEINFO ShExecInfo2;
	ShExecInfo2.cbSize = sizeof(SHELLEXECUTEINFO);
	ShExecInfo2.fMask = NULL;
	ShExecInfo2.hwnd = NULL;
	ShExecInfo2.lpVerb = L"runas";
	ShExecInfo2.lpFile = L"reagentc";
	ShExecInfo2.lpParameters = L"/boottore";
	ShExecInfo2.lpDirectory = system32fld.c_str();
	ShExecInfo2.nShow = SW_HIDE;
	ShExecInfo2.hInstApp = NULL;

	ShellExecuteExW(&ShExecInfo2);
	percentageCounter = 100;
	GUIDraw::updateProgressBar(percentageCounter, hProgressBar, hWnd, hProgressText);
	memset(hProgressText, 0, sizeof(hProgressText));
	WaitForSingleObject(ShExecInfo2.hProcess, INFINITE);
	CloseHandle(ShExecInfo2.hProcess);
	RestartDialogEx(NULL, L"TEST", EWX_REBOOT, NULL);
	exit(0);
	return 0;
}