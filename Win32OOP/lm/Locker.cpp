#include "Locker.h"
#include <iostream>

Locker& Locker::operator=(const Locker& rhs)
{
	this->assigned = rhs.assigned;
	this->date = rhs.date;
	this->enabled = rhs.enabled;
	this->hasKey = rhs.hasKey;
	this->isOwned = rhs.isOwned;
	this->lockerNum = rhs.lockerNum;
	this->previous = rhs.previous;
	this->status = rhs.status;

	return *this;
}

int Locker::GetLockerNumber() const
{
	return lockerNum;
}

std::string Locker::GetAssignedUser() const
{
	if(isOwned)
		return assigned;
	else
		return std::string("-----");
}

std::string Locker::GetPreviousUser() const
{
	return previous;
}

std::string Locker::GetStatusDescription() const
{
	return descStatus[status];
}

int Locker::GetStatusInt() const
{
	return status;
}
std::string Locker::GetDate() const
{
	return date;
}

bool Locker::HasKey() const
{
	return hasKey;
}

std::string Locker::HasKeyDescription() const
{
	return withKey[hasKey];
}

bool Locker::IsAssigned() const
{
	return isOwned;
}

bool Locker::Enabled() const
{
	return enabled;
}

void Locker::SetOwnership(bool owned)
{
	isOwned = owned;
}

void Locker::SetAssignedUser(const std::string& newUser)
{
	assigned = newUser;
}

void Locker::SetPreviousUser(const std::string& oldUser)
{
	previous = oldUser;
}

void Locker::SetDate(const std::string& newDate)
{
	date = newDate;
}

void Locker::SetLockerNumber(int lockerNumber)
{
	lockerNum = lockerNumber;
}

std::ostream& operator<<(std::ostream& stream, const Locker& data)
{
	stream << data.lockerNum << "|"
		<< data.assigned << "|"
		<< data.date << "|"
		<< data.previous << "|"
		<< data.isOwned << "|"
		<< data.hasKey << "|"
		<< data.status << "|"
		<< data.enabled << "|"; //\n optional, should not affect functionality

	return stream;
}

std::istream& operator>>(std::istream& stream, Locker& data)
{
	std::string temp;
	// lockerNum
	std::getline(stream, temp, '|'); // If the file is saved as UNICODE, must ignore first 3 bytes of the file

	if(stream.fail()) // for some weird reason, even the stream sets fail bit, it gives another loop (in CDocument::LoadData)
		return stream;// so, sa the data is emtpy, it fills and emtpy string, then it is passed to std::stoi which in turn
					  // throws and exception and exits inmediately from the app. This check prevents that.
					  // also, using std::atoi fixes that, shows no error and everything works fine.

	data.lockerNum = std::stoi(temp);

	std::getline(stream, data.assigned, '|');
	std::getline(stream, data.date, '|');
	std::getline(stream, data.previous, '|');
	// isOwned
	std::getline(stream, temp, '|');
	data.isOwned = std::stoi(temp); // 1 = true, 0 = false, who cares
	if(!data.isOwned)
		data.assigned = "";
	// hasKey
	std::getline(stream, temp, '|');
	data.hasKey = std::stoi(temp);
	// status
	std::getline(stream, temp, '|');
	data.status = std::stoi(temp);
	// enabled
	std::getline(stream, temp, '|');
	data.enabled = std::stoi(temp);
	//stream.ignore();

	return stream;
}

