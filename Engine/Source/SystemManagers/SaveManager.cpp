#include <fstream>

#include "SystemManagers/SaveManager.h"

SaveManager::SaveManager()
{
	if (std::ifstream File{ "Content/Save.json" })
	{
		Values = nlohmann::json::parse(File);
	}
	else
	{
		Values = nlohmann::json::object();
	}
}

SaveManager::~SaveManager()
{
	if (!Values.empty())
	{
		if (std::ofstream File{ "Content/Save.json" })
		{
			File << Values.dump(4);
		}
	}
}

bool SaveManager::Has(std::string_view Key) const
{
	return Values.contains(Key);
}

void SaveManager::Erase(std::string_view Key)
{
	Values.erase(Key);
}

void SaveManager::Clear()
{
	Values.clear();
}