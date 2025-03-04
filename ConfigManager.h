#pragma once

class ConfigManager {
public:
	static BOOL LoadConfig(std::wstring &ConfigFile);
private:
	static void SetValue(std::wstring &Key, std::wstring &Value);

	std::map<std::wstring, int> mapping = { { L"UpdateID", 1 },
	{ L"arch", 2 },
	{ L"requireupdate", 3 },
	{ L"OScommands", 4 } };
};