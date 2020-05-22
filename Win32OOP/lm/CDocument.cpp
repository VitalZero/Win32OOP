#include <stdexcept>
#include <fstream>
#include "CDocument.h"

void CDocument::SaveData(std::vector<Locker>& lockers)
{
	std::fstream saveData;

	saveData.open(path.c_str(), std::ios_base::out | std::ios_base::trunc);

	if (saveData.is_open())
	{
		saveData << lockers.size() << " " << nFileVersion << " ";

		for (unsigned int i = 0; i < lockers.size(); ++i)
		{
			saveData << lockers.at(i);
		}
	}
	else
	{
		throw std::runtime_error("Error!, could not save file :(");
	}
}

int CDocument::LoadData(std::vector<Locker>& lockers)
{
	std::fstream loadData;

	loadData.open(path.c_str(), std::ios_base::in);

	if (loadData.is_open())
	{
		int inFileVer = 0;

		loadData >> nRecords >> inFileVer;

		if (inFileVer != nFileVersion)
			throw std::runtime_error("Header file version doesn't match with current file version");

		loadData >> std::ws;
		loadData.clear();

		Locker tmp;
		lockers.clear();

		while ((loadData.good()) && (loadData >> tmp)) // here is the problem from Locker, even though is being checked in here
		{											// it gives another extra loop after the fail bit is set
			lockers.emplace_back(tmp);
		}

		if (nRecords == lockers.size())
			return nRecords;
		else
		{
			std::string strError = "Error!, Number of records loaded ";
			strError.append(std::to_string(lockers.size()));
			strError.append("doesn't match with number of records registered ");
			strError.append(std::to_string(nRecords));

			throw(std::runtime_error(strError));
		}
	}
	else
	{
		loadData.open(path, std::ios_base::out | std::ios_base::trunc);

		if (!loadData.is_open())
		{
			throw std::runtime_error(std::string("Error!, could not open or create file: " + path));
		}
		// set initially number of lockers = 0
		// file version = 2
		loadData << 0 << " " << nFileVersion << " ";

		return 0;
	}
}

unsigned int CDocument::NumberOfRecords() const
{
	return nRecords;
}
