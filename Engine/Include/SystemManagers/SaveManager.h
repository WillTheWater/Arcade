#pragma once

#include <nlohmann/json.hpp>
#include <string>

class SaveManager
{
public:
	SaveManager();
	~SaveManager();

	void Set(std::string_view Key, auto Value)
	{
		Values[Key] = Value;
	}

	auto Get(std::string_view Key, auto DefaultValues = {}) const
	{
		return Values.value(Key, DefaultValues);
	}

	bool Has(std::string_view Key) const;
	void Erase(std::string_view Key);
	void Clear();

private:
	nlohmann::json Values{};
};