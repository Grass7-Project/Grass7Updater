#pragma once

class FileManagement {
public:
	static void totalCallback(uint64_t total_size);
	static bool progressCallback(uint64_t Size);
	static BOOL extract(std::wstring &archivefile);
private:
	BOOL pwdProceed;
	uint64_t TotalSize;
};