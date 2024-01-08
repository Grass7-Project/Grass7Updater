// Functions related to file-management

#include "stdafx.h"
#include "FileManagement.h"
#include <string.h>
#include <bitextractor.hpp>
#include <bitexception.hpp>

FileManagementClass FileManagementObjects;

int FileManagementClass::extract(const char *filename)
{
	FileManagementObjects.pwdProceed = 0;
	wchar_t *archivefile = gr7::convertchar(filename);
	try {

		bit7z::Bit7zLibrary lib{
			L"7za.dll"
		};
		bit7z::BitExtractor extractor{ lib, bit7z::BitFormat::SevenZip };
		extractor.extract(archivefile, L"./");
		memset(archivefile, 0, sizeof(archivefile));
	}
	catch (const bit7z::BitException& ex) {
		ex.getErrorCode();
		return 1;
	}
	if (FileManagementObjects.pwdProceed != 1) {
		return 1;
	}
	return 0;
}