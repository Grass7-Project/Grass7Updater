#pragma once

// Main Class
class GlobalMain {
public:
	HWND hWndMainWindow;
	HINSTANCE hInst;
	std::wstring Error;
	std::wstring driveletter;
	std::wstring updatefld;
	std::wstring system32fld;
	std::wstring CommandLine;
	int ExtractionPercentage;
	ITaskbarList3* m_pITaskBarList3;
};

// Application Resource Strings
class GlobalAppResStrings {
public:
	std::wstring OSName;
	std::wstring PrivilageError;
	std::wstring NotInstalled;
	std::wstring FileListAccessError;
	std::wstring UpdateInProgress;
	std::wstring UpdaterError;
	std::wstring Question;
	std::wstring AlreadyInstalled;
	std::wstring ArchNotMatch;
	std::wstring ArchiveFail;
	std::wstring NoInternet;
	std::wstring UpdateReq1;
	std::wstring UpdateReq2;
	std::wstring FailOpenKey;
	std::wstring NotForThis;
	std::wstring ConfigAccessError;
	std::wstring Extracting;
};

class GlobalConfig {
public:
	std::wstring UpdateID;
	std::wstring Architecture;
	std::wstring RequiredUpdate;
	int OSCommands;
};

extern GlobalMain MainObjects;
extern GlobalAppResStrings AppResStringsObjects;
extern GlobalConfig ConfigObjects;