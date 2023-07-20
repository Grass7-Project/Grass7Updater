// The code to open and extract the up

#include "stdafx.h"
#include "FileManagement.h"
#include "MainCode.h"
#include "GUIDraw.h"
#include <algorithm>
#include <shlobj.h>
#include <regex>
#include <vector>

int mainCode(HWND hProgressBar, HWND hWnd, wchar_t *hProgressText, const wchar_t *lpCmdLine)
{
	char fgh[256];
	CHAR buffer2[256];
	size_t strlength;
	wcstombs_s(&strlength, buffer2, lpCmdLine, 256);
	std::string s(buffer2);
	s.erase(std::remove(s.begin(), s.end(), '"'), s.end());
	const char *updfile = s.c_str();
	Sleep(2000);
	if (gr7::GetSystemDriveLetter() == "") {
		TaskDialog(NULL, NULL, gr7::LoadStringToW(GetModuleHandle(NULL), IDS_OSNAME), gr7::LoadStringToW(GetModuleHandle(NULL), IDS_UPDATER_ERROR), gr7::LoadStringToW(GetModuleHandle(NULL), IDS_NOT_INSTALLED), TDCBF_CLOSE_BUTTON, TD_ERROR_ICON, NULL);
		memset(hProgressText, 0, sizeof(hProgressText));
		exit(0);
	}
	char bufferp[256];
	strncpy_s(bufferp, gr7::GetSystemDriveLetter(), sizeof(bufferp));
	strncat_s(bufferp, "gr7updatefld", sizeof(bufferp));
	CreateDirectory(gr7::convertchartoLPCWSTR(bufferp), NULL);
	SetCurrentDirectory(gr7::convertchar(bufferp));
	strncpy_s(fgh, gr7::GetSystemDriveLetter(), sizeof(fgh));
	strncat_s(fgh, "Windows\\System32", sizeof(fgh));
	int archiveerr = extract(updfile);
	if (archiveerr != 0) {
		SetCurrentDirectory(gr7::convertchar(fgh));
		memset(fgh, 0, sizeof(fgh));
		gr7::DeleteDirectory(gr7::convertchar(bufferp));
		TaskDialog(NULL, NULL, gr7::LoadStringToW(GetModuleHandle(NULL), IDS_OSNAME), gr7::LoadStringToW(GetModuleHandle(NULL), IDS_UPDATER_ERROR), gr7::LoadStringToW(GetModuleHandle(NULL), IDS_ARCHIVE_FAIL), TDCBF_CLOSE_BUTTON, TD_ERROR_ICON, NULL);
		memset(hProgressText, 0, sizeof(hProgressText));
		memset(bufferp, 0, sizeof(bufferp));
		exit(0);
	}

	// We load and parse the config file
	// We have to do a diarrhea way of doing this, not proud of this one, there is likely a better way but i was very angry at the time that it didnt work so i did diarrhea.
	char bufferpf[256];
	strncpy_s(bufferpf, gr7::GetSystemDriveLetter(), sizeof(bufferpf));
	strncat_s(bufferpf, "gr7updatefld\\Update.conf", sizeof(bufferpf));
	std::ifstream ifs(bufferpf);
	memset(bufferpf, 0, sizeof(bufferpf));
	std::string updateID = "UpdateID=";
	std::string arch = "arch=";
	std::string requiredupdate = "requiredupdate=";
	std::regex updateids(updateID + "\\b");
	std::regex archs(arch + "\\b");
	std::regex requiredupdates(requiredupdate + "\\b");

	std::string lineUpdateID;
	std::string lineArch;
	std::string lineRequiredupdate;

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
	ifs.close();
	updateID.clear();
	arch.clear();

	HKEY hkey1;
	HKEY hkey1d;
	char bufferreg[256];
	char bufferreg1[256];

	SetCurrentDirectory(gr7::convertchar(fgh));
	memset(fgh, 0, sizeof(fgh));

	std::string platformID1 = lineUpdateID.substr(0, lineUpdateID.find("-"));


	strncpy_s(bufferreg, "Software\\Grass7\\CurrentVersion\\Updates\\", sizeof(bufferreg));
	strncat_s(bufferreg, lineUpdateID.c_str(), sizeof(bufferreg));
	lineUpdateID.clear();
	strncpy_s(bufferreg1, "Software\\Grass7\\CurrentVersion\\Updates\\", sizeof(bufferreg));
	strncat_s(bufferreg1, lineRequiredupdate.c_str(), sizeof(bufferreg));
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
		TaskDialog(NULL, NULL, gr7::LoadStringToW(GetModuleHandle(NULL), IDS_OSNAME), gr7::LoadStringToW(GetModuleHandle(NULL), IDS_UPDATER_ERROR), gr7::LoadStringToW(GetModuleHandle(NULL), IDS_ARCH_NOT_MATCH), TDCBF_CLOSE_BUTTON, TD_ERROR_ICON, NULL);
		gr7::DeleteDirectory(gr7::convertchar(bufferp));
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
	LONG lResult = RegOpenKeyEx(HKEY_LOCAL_MACHINE, gr7::convertchartoLPCWSTR(bufferreg), 0, KEY_READ, &hkey1);
	memset(bufferreg, 0, sizeof(bufferreg));
	if (lResult == ERROR_SUCCESS)
	{
		TaskDialog(NULL, NULL, gr7::LoadStringToW(GetModuleHandle(NULL), IDS_OSNAME), gr7::LoadStringToW(GetModuleHandle(NULL), IDS_UPDATER_ERROR), gr7::LoadStringToW(GetModuleHandle(NULL), IDS_ALREADY_INSTALLED), TDCBF_CLOSE_BUTTON, TD_ERROR_ICON, NULL);
		gr7::DeleteDirectory(gr7::convertchar(bufferp));
		memset(bufferp, 0, sizeof(bufferp));
		memset(hProgressText, 0, sizeof(hProgressText));
		lineRequiredupdate.clear();
		lineRequiredupdateW.clear();
		platformID1.clear();
		platformID1W.clear();
		exit(0);
	}
	LONG lResult1 = RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("Software\\Grass7\\CurrentVersion"), 0, KEY_READ, &hkey1d);
	if (lResult1 != ERROR_SUCCESS)
	{
		TaskDialog(NULL, NULL, gr7::LoadStringToW(GetModuleHandle(NULL), IDS_OSNAME), gr7::LoadStringToW(GetModuleHandle(NULL), IDS_UPDATER_ERROR), gr7::LoadStringToW(GetModuleHandle(NULL), IDS_FAIL_OPEN_KEY), TDCBF_CLOSE_BUTTON, TD_ERROR_ICON, NULL);
		gr7::DeleteDirectory(gr7::convertchar(bufferp));
		memset(bufferp, 0, sizeof(bufferp));
		memset(hProgressText, 0, sizeof(hProgressText));
		lineRequiredupdate.clear();
		lineRequiredupdateW.clear();
		platformID1.clear();
		platformID1W.clear();
		exit(0);
	}

	WCHAR szBuffer[64];
	DWORD dwBufferSize = sizeof(szBuffer);
	ULONG nError;
	nError = RegQueryValueExW(hkey1d,_T("UpdateLevel"), 0, NULL, (LPBYTE)szBuffer, &dwBufferSize);

	wchar_t szBuffer1[64];

	wcscpy_s(szBuffer1, _T("GR7U"));
	wcsncat_s(szBuffer1, szBuffer, 24);

	std::wstring szBuffer1WS = szBuffer1;

	if (platformID1W.find(szBuffer1WS) == std::string::npos) {
		TaskDialog(NULL, NULL, gr7::LoadStringToW(GetModuleHandle(NULL), IDS_OSNAME), gr7::LoadStringToW(GetModuleHandle(NULL), IDS_UPDATER_ERROR), gr7::LoadStringToW(GetModuleHandle(NULL), IDS_NOT_FOR_THIS), TDCBF_CLOSE_BUTTON, TD_ERROR_ICON, NULL);
		gr7::DeleteDirectory(gr7::convertchar(bufferp));
		memset(bufferp, 0, sizeof(bufferp));
		memset(hProgressText, 0, sizeof(hProgressText));
		memset(szBuffer1, 0, sizeof(szBuffer1));
		lineRequiredupdate.clear();
		lineRequiredupdateW.clear();
		platformID1.clear();
		platformID1W.clear();
		exit(0);
	}

	if(lineRequiredupdate.find("none") == std::string::npos) {
		LONG lResult = RegOpenKeyEx(HKEY_LOCAL_MACHINE, gr7::convertchartoLPCWSTR(bufferreg1), 0, KEY_READ, &hkey1);
		memset(bufferreg1, 0, sizeof(bufferreg1));
		if (lResult != ERROR_SUCCESS) 
		{
			TCHAR ErrorString[MAX_PATH];
			
			wcscpy_s(ErrorString, MAX_PATH, gr7::LoadStringToW(GetModuleHandle(NULL), IDS_UPD_REQ1));
			wcscat_s(ErrorString,MAX_PATH,RequiredUpd);
			wcscat_s(ErrorString,MAX_PATH,gr7::LoadStringToW(GetModuleHandle(NULL),IDS_UPD_REQ2));
			TaskDialog(NULL, NULL, gr7::LoadStringToW(GetModuleHandle(NULL),IDS_OSNAME),  gr7::LoadStringToW(GetModuleHandle(NULL),IDS_UPDATER_ERROR), ErrorString, TDCBF_CLOSE_BUTTON, TD_ERROR_ICON, NULL);
			gr7::DeleteDirectory(gr7::convertchar(bufferp));
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
	SetCurrentDirectory(gr7::convertchar(bufferp));
	memset(bufferp, 0, sizeof(bufferp));
	CreateQuestion();
	::ShowWindow(hWnd, SW_SHOWDEFAULT);
	::UpdateWindow(hWnd);
	int percentageCounter = 0;
	updateProgressBar(percentageCounter, hProgressBar, hWnd, hProgressText);
	// We reboot to winRE
	SHELLEXECUTEINFO ShExecInfo;
    ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
    ShExecInfo.fMask = NULL;
    ShExecInfo.hwnd = NULL;
    ShExecInfo.lpVerb = _T("runas");
    ShExecInfo.lpFile = _T("reagentc");
    ShExecInfo.lpParameters = _T("/boottore");
    ShExecInfo.lpDirectory = NULL;
    ShExecInfo.nShow = SW_HIDE;
    ShExecInfo.hInstApp = NULL;

    ShellExecuteEx(&ShExecInfo);
	percentageCounter = 100;
	updateProgressBar(percentageCounter, hProgressBar, hWnd, hProgressText);
	memset(hProgressText, 0, sizeof(hProgressText));
	RestartDialogEx(NULL, _T("TEST"), EWX_REBOOT, NULL);
	exit(0);
	return 0;
}
