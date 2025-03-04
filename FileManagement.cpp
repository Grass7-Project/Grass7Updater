#include "stdafx.h"
#include "FileManagement.h"
#include "GUIDraw.h"
#include <bit7z/bitfileextractor.hpp>
#include <bit7z/bitexception.hpp>

FileManagement FileManagementObjects;

void FileManagement::totalCallback(uint64_t total_size)
{
	FileManagementObjects.TotalSize = total_size;
}

bool FileManagement::progressCallback(uint64_t progress_size)
{
	int percentage = int(progress_size * 100 / FileManagementObjects.TotalSize);
	if (MainObjects.ExtractionPercentage != percentage) {
		MainObjects.ExtractionPercentage = percentage;
		RedrawWindow(MainObjects.hWndMainWindow, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
		MainObjects.m_pITaskBarList3->SetProgressValue(MainObjects.hWndMainWindow, progress_size, FileManagementObjects.TotalSize);
	}
	return TRUE;
}

BOOL FileManagement::extract(std::wstring &archivefile)
{
	using convert_type = std::codecvt_utf8<wchar_t>;
	std::wstring_convert<convert_type, wchar_t> converter;

	std::string str_archivefile = converter.to_bytes(archivefile);

	FileManagementObjects.pwdProceed = FALSE;
	FileManagementObjects.TotalSize = 0;
	try {

		bit7z::Bit7zLibrary lib{
			"7za.dll"
		};
		bit7z::BitFileExtractor extractor{ lib, bit7z::BitFormat::SevenZip };

		extractor.setTotalCallback(FileManagement::totalCallback);
		extractor.setProgressCallback(FileManagement::progressCallback);
		extractor.extract(str_archivefile, "./");
	}
	catch (const bit7z::BitException& ex) {
		UNREFERENCED_PARAMETER(ex);
		return 1;
	}
	if (FileManagementObjects.pwdProceed != TRUE) {
		return 1;
	}
	return 0;
}