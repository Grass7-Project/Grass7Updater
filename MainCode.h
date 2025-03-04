#pragma once

class MainCode {
public:
	static void mainCode(std::wstring &UpdateFile);
private:
	std::wstring UpdateConfigFile;
	std::wstring UpdateIDRegistry;
	std::wstring RequiredUpdateRegistry;
};