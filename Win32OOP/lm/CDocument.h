#ifndef CDOCUMENT_H_
#define CDOCUMENT_H_
/*
 * CDocument.h
 * It is used by CLockManager.
 * Just used to load and save data from a file using a vector of CLockers
 * CLockers overloads stream operators >> and <<
 * Just feed a vector of CLockers and it's all good!
 */
#include <string>
#include <vector>
#include "Locker.h"

class CDocument {
public:
	CDocument() = delete;
	CDocument(const CDocument&) = delete;
	CDocument& operator=(const CDocument&) = delete;
	CDocument(const std::string& path_in)
		: path(path_in), nRecords(0)
	{
	}
	void SaveData(std::vector<Locker>& lockers);
	int LoadData(std::vector<Locker>& lockers);
	unsigned int NumberOfRecords() const;

private:
	const std::string path;
	static constexpr int nFileVersion = 2;
	unsigned int nRecords;
};

#endif /* CDOCUMENT_H_ */
