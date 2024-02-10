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
	WCHAR driveletter[MAX_PATH];
	gr7::GetSystemDriveLetterW(driveletter);
	wchar_t fgh[MAX_PATH] = { 0 };
	CHAR buffer2[MAX_PATH];
	size_t strlength;
	wcstombs_s(&strlength, buffer2, lpCmdLine, 256);
	std::string s(buffer2);
	s.erase(std::remove(s.begin(), s.end(), '"'), s.end());
	const char *updfile = s.c_str();
	Sleep(2000);
	if (driveletter == L"") {
		TaskDialog(NULL, NULL, AppResStringsObjects.OSName, AppResStringsObjects.UpdaterError, AppResStringsObjects.NotInstalled, TDCBF_CLOSE_BUTTON, TD_ERROR_ICON, NULL);
		memset(hProgressText, 0, sizeof(hProgressText));
		exit(0);
	}
	wchar_t bufferp[MAX_PATH] = { 0 };
	wcsncpy_s(bufferp, driveletter, sizeof(bufferp));
	wcsncat_s(bufferp, L"gr7updatefld", sizeof(bufferp));
	CreateDirectoryW(bufferp, NULL);
	SetCurrentDirectoryW(bufferp);
	wcsncpy_s(fgh, driveletter, sizeof(fgh));
	wcsncat_s(fgh, L"Windows\\System32", sizeof(fgh));
	int archiveerr = FileManagementClass::extract(updfile);
	if (archiveerr != 0) {
		SetCurrentDirectoryW(fgh);
		memset(fgh, 0, sizeof(fgh));
		gr7::DeleteDirectory(bufferp);
		TaskDialog(NULL, NULL, AppResStringsObjects.OSName, AppResStringsObjects.UpdaterError, AppResStringsObjects.ArchiveFail, TDCBF_CLOSE_BUTTON, TD_ERROR_ICON, NULL);
		memset(hProgressText, 0, sizeof(hProgressText));
		memset(bufferp, 0, sizeof(bufferp));
		exit(0);
	}

	// We load and parse the config file
	// We have to do a diarrhea way of doing this, not proud of this one, there is likely a better way but i was very angry at the time that it didnt work so i did this fucking shit.
	WCHAR bufferpf[256];
	wcsncpy_s(bufferpf, driveletter, sizeof(bufferpf));
	wcsncat_s(bufferpf, L"gr7updatefld\\Update.conf", sizeof(bufferpf));
	std::ifstream ifs(bufferpf);
	memset(bufferpf, 0, sizeof(bufferpf));
	std::string updateID = "UpdateID=";
	std::string arch = "arch=";
	std::string requiredupdate = "requiredupdate=";
	std::string OScmd = "OScommands=";
	std::regex updateids(updateID + "\\b");
	std::regex archs(arch + "\\b");
	std::regex requiredupdates(requiredupdate + "\\b");
	std::regex OScmden(OScmd + "\\b");

	std::string lineUpdateID;
	std::string lineArch;
	std::string lineRequiredupdate;
	std::string lineOScommands;

	getline(ifs, lineUpdateID);
	if (regex_search(lineUpdateID, updateids)) {
		std::size_t pos = lineUpdateID.find("=");
		if (pos != std::string::npos)
		{
			lineUpdateID = lineUpdateID.substr(pos + 1);
		}
	}
	getline(ifs, lineArch);
	if (regex_search(lineArch, archs)) {
		std::size_t pos = lineArch.find("=");
		if (pos != std::string::npos)
		{
			lineArch = lineArch.substr(pos + 1);
		}
	}
	getline(ifs, lineRequiredupdate);
	if (regex_search(lineRequiredupdate, requiredupdates)) {
		std::size_t pos = lineRequiredupdate.find("=");
		if (pos != std::string::npos)
		{
			lineRequiredupdate = lineRequiredupdate.substr(pos + 1);
		}
	}
	int enableOScommands = 0;
	getline(ifs, lineOScommands);
	if (regex_search(lineOScommands, OScmden)) {
		if (lineOScommands.find("true") != std::string::npos) {
			enableOScommands = 1;
		}
	}
	ifs.close();
	lineOScommands.clear();
	updateID.clear();
	arch.clear();

	HKEY hkey1;
	HKEY hkey1d;
	wchar_t bufferreg[256] = { 0 };
	wchar_t bufferreg1[256] = { 0 };

	SetCurrentDirectoryW(fgh);
	memset(fgh, 0, sizeof(fgh));

	std::string platformID1 = lineUpdateID.substr(0, lineUpdateID.find("-"));


	wcsncpy_s(bufferreg, L"Software\\Grass7\\CurrentVersion\\Updates\\", sizeof(bufferreg));
	wcsncat_s(bufferreg, gr7::convertchar(lineUpdateID.c_str()), sizeof(bufferreg));
	lineUpdateID.clear();
	wcsncpy_s(bufferreg1, L"Software\\Grass7\\CurrentVersion\\Updates\\", sizeof(bufferreg));
	wcsncat_s(bufferreg1, gr7::convertchar(lineRequiredupdate.c_str()), sizeof(bufferreg));
	std::wstring lineRequiredupdateW = std::wstring(lineRequiredupdate.begin(), lineRequiredupdate.end());
	std::wstring platformID1W = std::wstring(platformID1.begin(), platformID1.end());
	const wchar_t *RequiredUpd = lineRequiredupdateW.c_str();


#if defined(_M_X64)
	const char *specificarch = "amd64";
#endif
#if defined(_M_IX86)
	const char *specificarch = "i386";
#endif

	if (lineArch != specificarch) {
		TaskDialog(NULL, NULL, AppResStringsObjects.OSName, AppResStringsObjects.UpdaterError, AppResStringsObjects.ArchNotMatch, TDCBF_CLOSE_BUTTON, TD_ERROR_ICON, NULL);
		gr7::DeleteDirectory(bufferp);
		memset(bufferp, 0, sizeof(bufferp));
		memset(hProgressText, 0, sizeof(hProgressText));
		lineArch.clear();
		lineRequiredupdate.clear();
		lineRequiredupdateW.clear();
		platformID1.clear();
		platformID1W.clear();
		exit(0);
	}
	lineArch.clear();

	// Registry stuff
	LONG lResult = RegOpenKeyExW(HKEY_LOCAL_MACHINE, bufferreg, 0, KEY_READ, &hkey1);
	memset(bufferreg, 0, sizeof(bufferreg));
	if (lResult == ERROR_SUCCESS)
	{
		TaskDialog(NULL, NULL, AppResStringsObjects.OSName, AppResStringsObjects.UpdaterError, AppResStringsObjects.AlreadyInstalled, TDCBF_CLOSE_BUTTON, TD_ERROR_ICON, NULL);
		gr7::DeleteDirectory(bufferp);
		memset(bufferp, 0, sizeof(bufferp));
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
		TaskDialog(NULL, NULL, AppResStringsObjects.OSName, AppResStringsObjects.UpdaterError, AppResStringsObjects.FailOpenKey, TDCBF_CLOSE_BUTTON, TD_ERROR_ICON, NULL);
		gr7::DeleteDirectory(bufferp);
		memset(bufferp, 0, sizeof(bufferp));
		memset(hProgressText, 0, sizeof(hProgressText));
		lineRequiredupdate.clear();
		lineRequiredupdateW.clear();
		platformID1.clear();
		platformID1W.clear();
		exit(0);
	}

	WCHAR szBuffer[512];
	DWORD dwBufferSize = sizeof(szBuffer);
	ULONG nError;
	nError = RegQueryValueExW(hkey1d, L"UpdateLevel", 0, NULL, (LPBYTE)szBuffer, &dwBufferSize);

	wchar_t szBuffer1[64] = { 0 };

	wcscpy_s(szBuffer1, L"GR7U");
	wcsncat_s(szBuffer1, szBuffer, sizeof(szBuffer));

	std::wstring szBuffer1WS = szBuffer1;

	if (platformID1W.find(szBuffer1WS) == std::string::npos) {
		TaskDialog(NULL, NULL, AppResStringsObjects.OSName, AppResStringsObjects.UpdaterError, AppResStringsObjects.NotForThis, TDCBF_CLOSE_BUTTON, TD_ERROR_ICON, NULL);
		gr7::DeleteDirectory(bufferp);
		memset(bufferp, 0, sizeof(bufferp));
		memset(hProgressText, 0, sizeof(hProgressText));
		memset(szBuffer1, 0, sizeof(szBuffer1));
		lineRequiredupdate.clear();
		lineRequiredupdateW.clear();
		platformID1.clear();
		platformID1W.clear();
		exit(0);
	}

	if (lineRequiredupdate.find("none") == std::string::npos) {
		LONG lResult = RegOpenKeyExW(HKEY_LOCAL_MACHINE, bufferreg1, 0, KEY_READ, &hkey1);
		memset(bufferreg1, 0, sizeof(bufferreg1));
		if (lResult != ERROR_SUCCESS)
		{
			TCHAR ErrorString[MAX_PATH];

			wcscpy_s(ErrorString, sizeof(ErrorString), AppResStringsObjects.UpdateReq1);
			wcscat_s(ErrorString, sizeof(ErrorString), RequiredUpd);
			wcscat_s(ErrorString, sizeof(ErrorString), AppResStringsObjects.UpdateReq2);
			TaskDialog(NULL, NULL, AppResStringsObjects.OSName, AppResStringsObjects.UpdaterError, ErrorString, TDCBF_CLOSE_BUTTON, TD_ERROR_ICON, NULL);
			gr7::DeleteDirectory(bufferp);
			memset(bufferp, 0, sizeof(bufferp));
			memset(hProgressText, 0, sizeof(hProgressText));
			lineRequiredupdate.clear();
			lineRequiredupdateW.clear();
			platformID1.clear();
			platformID1W.clear();
			exit(0);
		}
	}

	lineRequiredupdate.clear();
	lineRequiredupdateW.clear();
	platformID1.clear();
	platformID1W.clear();
	memset(bufferp, 0, sizeof(bufferp));
	GUIDraw::CreateQuestion();
	::ShowWindow(hWnd, SW_SHOWDEFAULT);
	::UpdateWindow(hWnd);
	int percentageCounter = 0;
	GUIDraw::updateProgressBar(percentageCounter, hProgressBar, hWnd, hProgressText);

	wchar_t sysdir[256] = { 0 };
	wcsncpy_s(sysdir, driveletter, sizeof(sysdir));
	wcsncat_s(sysdir, L"Windows\\System32", sizeof(sysdir));
	SetCurrentDirectoryW(bufferp);

	if (enableOScommands == 1) {
		wchar_t cmddlol[256] = { 0 };
		wcsncpy_s(cmddlol, driveletter, sizeof(cmddlol));
		wcsncat_s(cmddlol, L"gr7updatefld\\OScommands.bat", sizeof(cmddlol));

		SHELLEXECUTEINFO ShExecInfo;
		ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
		ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
		ShExecInfo.hwnd = NULL;
		ShExecInfo.lpVerb = L"open";
		ShExecInfo.lpFile = cmddlol;
		ShExecInfo.lpParameters = L"";
		ShExecInfo.lpDirectory = sysdir;
		ShExecInfo.nShow = SW_HIDE;
		ShExecInfo.hInstApp = NULL;
		ShellExecuteExW(&ShExecInfo);
		WaitForSingleObject(ShExecInfo.hProcess, INFINITE);
		CloseHandle(ShExecInfo.hProcess);
		memset(cmddlol, 0, sizeof(cmddlol));
	}

	// We reboot to winRE
	SHELLEXECUTEINFO ShExecInfo2;
	ShExecInfo2.cbSize = sizeof(SHELLEXECUTEINFO);
	ShExecInfo2.fMask = NULL;
	ShExecInfo2.hwnd = NULL;
	ShExecInfo2.lpVerb = L"runas";
	ShExecInfo2.lpFile = L"reagentc";
	ShExecInfo2.lpParameters = L"/boottore";
	ShExecInfo2.lpDirectory = sysdir;
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