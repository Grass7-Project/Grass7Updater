#include "stdafx.h"
#include "ResourceLoader.h"

void ResourceLoader::LoadStrings()
{
	InternalLoadString(AppResStringsObjects.OSName, IDS_OSNAME);
	InternalLoadString(AppResStringsObjects.PrivilageError, IDS_PRIVILAGE_ERROR);
	InternalLoadString(AppResStringsObjects.NotInstalled, IDS_NOT_INSTALLED);
	InternalLoadString(AppResStringsObjects.FileListAccessError, IDS_FILE_LIST_ACCESS_ERROR);
	InternalLoadString(AppResStringsObjects.UpdateInProgress, IDS_UPDATE_IN_PROGRESS);
	InternalLoadString(AppResStringsObjects.UpdaterError, IDS_UPDATER_ERROR);
	InternalLoadString(AppResStringsObjects.Question, IDS_QUESTION);
	InternalLoadString(AppResStringsObjects.AlreadyInstalled, IDS_ALREADY_INSTALLED);
	InternalLoadString(AppResStringsObjects.ArchNotMatch, IDS_ARCH_NOT_MATCH);
	InternalLoadString(AppResStringsObjects.ArchiveFail, IDS_ARCHIVE_FAIL);
	InternalLoadString(AppResStringsObjects.NoInternet, IDS_NO_INTERNET);
	InternalLoadString(AppResStringsObjects.UpdateReq1, IDS_UPD_REQ1);
	InternalLoadString(AppResStringsObjects.UpdateReq2, IDS_UPD_REQ2);
	InternalLoadString(AppResStringsObjects.FailOpenKey, IDS_FAIL_OPEN_KEY);
	InternalLoadString(AppResStringsObjects.NotForThis, IDS_NOT_FOR_THIS);
	InternalLoadString(AppResStringsObjects.ConfigAccessError, IDS_CONFIG_ACCESS_ERROR);
	InternalLoadString(AppResStringsObjects.Extracting, IDS_EXTRACTING);
}

void ResourceLoader::InternalLoadString(std::wstring &strReturn, UINT uID)
{
	std::wstring strTemp(MAX_PATH, 0);
	strTemp.resize((size_t)LoadStringW(MainObjects.hInst, uID, &strTemp[0], (int)strTemp.size()));
	strReturn = strTemp;
	strTemp.clear();
}