// Functions related to file-management

#include "stdafx.h"
#include "FileManagement.h"
#include <string.h>
#include <bitextractor.hpp>
#include <bitexception.hpp>

int extract(const char *filename)
{
	wchar_t *archivefile = gr7::convertchar(filename);
	try {

		bit7z::Bit7zLibrary lib{
			_T("7za.dll")
		};
		bit7z::BitExtractor extractor{ lib, bit7z::BitFormat::SevenZip };
		
		extractor.extract(gr7::convertchar(filename), _T("./"));
		memset(archivefile, 0, sizeof(archivefile));
	}
	catch (const bit7z::BitException& ex) {
		ex.getErrorCode();
		return 1;
	}
	return 0;
}
