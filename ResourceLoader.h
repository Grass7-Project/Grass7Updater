#pragma once

class ResourceLoader {
public:
	static void LoadStrings();
private:
	static void InternalLoadString(std::wstring &strReturn, UINT uID);
};