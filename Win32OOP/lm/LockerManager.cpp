#include <iostream>
#include <stdexcept>
#include <ctime>
#include <algorithm>
#include "LockerManager.h"

LockerManager::LockerManager(const std::string& path_in)
	:
	doc(path_in),
	totalLockers(0)
{
	std::time_t now;

	std::time(&now);
	std::tm ltm = { 0 };
	localtime_s(&ltm, &now);

 	currDate = (ltm.tm_mday < 10 ? "0" : "") + std::to_string(ltm.tm_mday);
	currDate += "/";
	currDate += (ltm.tm_mon < 9 ? "0" : "") + std::to_string(ltm.tm_mon+1);
	currDate += "/";
	currDate += std::to_string(ltm.tm_year+1900);

	LoadData();
}

int LockerManager::AddLocker()
{
	Locker tmpLocker;
	tmpLocker.SetLockerNumber(GetTotalLockersQty() + 1);

	tmpLocker.SetDate(currDate);
	tmpLocker.SetOwnership(false);
	lockers.push_back(tmpLocker);

	if(SaveChanges())
		return tmpLocker.GetLockerNumber();
	else
		return 0;
}

int LockerManager::GetLockers(std::vector<Locker>& lockers_in, Locker::Filter show)
{
	// TODO: Filter input
	lockers_in.clear();

	switch(show)
	{
		case Locker::Filter::All:
			lockers_in = lockers;
		break;
		case Locker::Filter::Assigned:
		{
			std::copy_if(lockers.begin(), lockers.end(), std::back_inserter(lockers_in),
				[](const Locker& l)
				{
					return l.IsAssigned();
				});
		}
		break;
		case Locker::Filter::NotAssigned:
		{
			std::copy_if(lockers.begin(), lockers.end(), std::back_inserter(lockers_in),
				[](const Locker& l)
				{
					return !l.IsAssigned();
				});
		}
		break;
		case Locker::Filter::Good:
		{
			std::copy_if(lockers.begin(), lockers.end(), std::back_inserter(lockers_in),
				[](const Locker& l)
				{
					return !l.GetStatusInt() == 0;
				});
		}
		break;
		case Locker::Filter::Bad:
		case Locker::Filter::Disabled:
		case Locker::Filter::Enabled:
		case Locker::Filter::NoKey:
		case Locker::Filter::WithKey:
		break;
		default:
			return 0;
		break;
	}

	// result defaults to false, true will be set only if there are matches
	return lockers_in.size();
}

int LockerManager::GetTotalLockersQty() const
{
	return lockers.size();
}

bool LockerManager::ChangeUser(int locker_in,const std::string& newAssigned, bool owned)
{
	const int foundPos = Search(locker_in);

	if( (foundPos >= 0) && (lockers.size() > 0) )
	{
		std::string tmp = lockers.at(foundPos).GetAssignedUser();

		lockers.at(foundPos).SetAssignedUser(ToUpper(newAssigned));
		lockers.at(foundPos).SetPreviousUser(ToUpper(tmp));
		lockers.at(foundPos).SetDate(currDate);
		lockers.at(foundPos).SetOwnership(owned);

		return SaveChanges();
	}

	return false;
}

bool LockerManager::DeleteUser(int locker_in)
{
	return ChangeUser(locker_in, "", false);
}

bool LockerManager::SaveChanges()
{
	doc.SaveData(lockers);
	LoadData();

	return true;
}

int LockerManager::Search(int numLocker_in)
{
	const int NORESULTS = -1;
	for(unsigned int i = 0; i < lockers.size(); ++i)
	{
		if(lockers.at(i).GetLockerNumber() == numLocker_in)
		{
			return i;
		}
	}
	return NORESULTS;
}

bool LockerManager::SearchUser(const std::string& user, std::vector<Locker>& lockers_in )
{
	std::string tmpUser = ToUpper(user);
	lockers_in.clear();

	std::copy_if(lockers.begin(), lockers.end(), std::back_inserter(lockers_in), 
		[&tmpUser](const Locker& l)
	{
		return std::string::npos != l.GetAssignedUser().find(tmpUser);
	});

	if( lockers_in.size() > 0)
	{
		return true;
	}
	
	return false;
}

std::string LockerManager::ToUpper(const std::string& str_in)
{
	std::string tmp = str_in;
	std::transform(str_in.begin(), str_in.end(), tmp.begin(), ::toupper );

	return tmp;
}

void LockerManager::LoadData()
{
	totalLockers = doc.LoadData(lockers);
}
