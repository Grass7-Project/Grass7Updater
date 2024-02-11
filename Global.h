#pragma once

// Main Class
class GlobalMain {
public:
	HINSTANCE hInst;
};

// Application Resource Strings
class GlobalAppResStrings {
public:
	std::wstring OSName;
	std::wstring PrivilageError;
	std::wstring NotInstalled;
	std::wstring Installing;
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
};

extern GlobalMain MainObjects;
extern GlobalAppResStrings AppResStringsObjects;