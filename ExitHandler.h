#pragma once

class ExitHandler {
public:
	static void Cleanup();
	static void ErrorMessage(std::wstring &ErrorString);
};