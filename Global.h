#pragma once

// Main Class
class GlobalMain {
public:
	HINSTANCE hInst;
};

// Application Resource Strings
class GlobalAppResStrings {
public:
	wchar_t *OSName;
	wchar_t *PrivilageError;
	wchar_t *NotInstalled;
	wchar_t *Installing;
	wchar_t *FileListAccessError;
	wchar_t *UpdateInProgress;
	wchar_t *UpdaterError;
	wchar_t *Question;
	wchar_t *AlreadyInstalled;
	wchar_t *ArchNotMatch;
	wchar_t *ArchiveFail;
	wchar_t *NoInternet;
	wchar_t *UpdateReq1;
	wchar_t *UpdateReq2;
	wchar_t *FailOpenKey;
	wchar_t *NotForThis;
};

extern GlobalMain MainObjects;
extern GlobalAppResStrings AppResStringsObjects;