#include "stdafx.h"
#include "ConfigManager.h"

ConfigManager ConfigManagerObjects;

BOOL ConfigManager::LoadConfig(std::wstring &ConfigFile)
{
	std::wifstream file(ConfigFile);

	if (!file.is_open()) {
		return 1;
	}

	std::wstring line;
	while (std::getline(file, line))
	{
		std::wistringstream iss(line);
		std::wstring key;
		if (std::getline(iss, key, L'='))
		{
			std::wstring value;
			if (std::getline(iss, value)) {
				SetValue(key, value);
			}
		}
	}
	file.close();
	return 0;
}

void ConfigManager::SetValue(std::wstring &Key, std::wstring &Value)
{
	std::map<std::wstring, int>::const_iterator iter =
		ConfigManagerObjects.mapping.find(Key);

	if (iter != ConfigManagerObjects.mapping.end())
	{
		switch (iter->second)
		{
		case 1:
			ConfigObjects.UpdateID = Value;
			break;
		case 2:
			ConfigObjects.Architecture = Value;
			break;
		case 3:
			ConfigObjects.RequiredUpdate = Value;
			break;
		case 4:
			if (Value.find(L"true") != std::wstring::npos)
				ConfigObjects.OSCommands = TRUE;
			break;
		}
	}
}