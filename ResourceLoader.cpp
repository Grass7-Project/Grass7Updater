#include "stdafx.h"
#include "ResourceLoader.h"
#include "Global.h"

void ResourceLoader::LoadStrings()
{
	std::wstring OSName(MAX_PATH, 0);
	std::wstring PrivilageError(MAX_PATH, 0);
	std::wstring NotInstalled(MAX_PATH, 0);
	std::wstring Installing(MAX_PATH, 0);
	std::wstring FileListAccessError(MAX_PATH, 0);
	std::wstring UpdateInProgress(MAX_PATH, 0);
	std::wstring UpdaterError(MAX_PATH, 0);
	std::wstring Question(MAX_PATH, 0);
	std::wstring AlreadyInstalled(MAX_PATH, 0);
	std::wstring ArchNotMatch(MAX_PATH, 0);
	std::wstring ArchiveFail(MAX_PATH, 0);
	std::wstring NoInternet(MAX_PATH, 0);
	std::wstring UpdateReq1(MAX_PATH, 0);
	std::wstring UpdateReq2(MAX_PATH, 0);
	std::wstring FailOpenKey(MAX_PATH, 0);
	std::wstring NotForThis(MAX_PATH, 0);

	OSName.resize((size_t)LoadStringW(MainObjects.hInst, IDS_OSNAME, &OSName[0], (int)OSName.size()));
	PrivilageError.resize((size_t)LoadStringW(MainObjects.hInst, IDS_PRIVILAGE_ERROR, &PrivilageError[0], (int)PrivilageError.size()));
	NotInstalled.resize((size_t)LoadStringW(MainObjects.hInst, IDS_NOT_INSTALLED, &NotInstalled[0], (int)NotInstalled.size()));
	Installing.resize((size_t)LoadStringW(MainObjects.hInst, IDS_INSTALLING, &Installing[0], (int)Installing.size()));
	FileListAccessError.resize((size_t)LoadStringW(MainObjects.hInst, IDS_FILE_LIST_ACCESS_ERROR, &FileListAccessError[0], (int)FileListAccessError.size()));
	UpdateInProgress.resize((size_t)LoadStringW(MainObjects.hInst, IDS_UPDATE_IN_PROGRESS, &UpdateInProgress[0], (int)UpdateInProgress.size()));
	UpdaterError.resize((size_t)LoadStringW(MainObjects.hInst, IDS_UPDATER_ERROR, &UpdaterError[0], (int)UpdaterError.size()));
	Question.resize((size_t)LoadStringW(MainObjects.hInst, IDS_QUESTION, &Question[0], (int)Question.size()));
	AlreadyInstalled.resize((size_t)LoadStringW(MainObjects.hInst, IDS_ALREADY_INSTALLED, &AlreadyInstalled[0], (int)AlreadyInstalled.size()));
	ArchNotMatch.resize((size_t)LoadStringW(MainObjects.hInst, IDS_ARCH_NOT_MATCH, &ArchNotMatch[0], (int)ArchNotMatch.size()));
	ArchiveFail.resize((size_t)LoadStringW(MainObjects.hInst, IDS_ARCHIVE_FAIL, &ArchiveFail[0], (int)ArchiveFail.size()));
	NoInternet.resize((size_t)LoadStringW(MainObjects.hInst, IDS_NO_INTERNET, &NoInternet[0], (int)NoInternet.size()));
	UpdateReq1.resize((size_t)LoadStringW(MainObjects.hInst, IDS_UPD_REQ1, &UpdateReq1[0], (int)UpdateReq1.size()));
	UpdateReq2.resize((size_t)LoadStringW(MainObjects.hInst, IDS_UPD_REQ2, &UpdateReq2[0], (int)UpdateReq2.size()));
	FailOpenKey.resize((size_t)LoadStringW(MainObjects.hInst, IDS_FAIL_OPEN_KEY, &FailOpenKey[0], (int)FailOpenKey.size()));
	NotForThis.resize((size_t)LoadStringW(MainObjects.hInst, IDS_NOT_FOR_THIS, &NotForThis[0], (int)NotForThis.size()));


	AppResStringsObjects.OSName = OSName;
	AppResStringsObjects.PrivilageError = PrivilageError;
	AppResStringsObjects.NotInstalled = NotInstalled;
	AppResStringsObjects.Installing = Installing;
	AppResStringsObjects.FileListAccessError = FileListAccessError;
	AppResStringsObjects.UpdateInProgress = UpdateInProgress;
	AppResStringsObjects.UpdaterError = UpdaterError;
	AppResStringsObjects.Question = Question;
	AppResStringsObjects.AlreadyInstalled = AlreadyInstalled;
	AppResStringsObjects.ArchNotMatch = ArchNotMatch;
	AppResStringsObjects.ArchiveFail = ArchiveFail;
	AppResStringsObjects.NoInternet = NoInternet;
	AppResStringsObjects.UpdateReq1 = UpdateReq1;
	AppResStringsObjects.UpdateReq2 = UpdateReq2;
	AppResStringsObjects.FailOpenKey = FailOpenKey;
	AppResStringsObjects.NotForThis = NotForThis;
}