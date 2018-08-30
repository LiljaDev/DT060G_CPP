// File: MyTime.cpp
// Written by: Johan Lilja
// Date: 2015-10-25
// Last edited: 2015-11-01
// Course: DT060G OOP C++
// Assignment: Project Trains
#include "MyTime.h"
#include <string>
#include <sstream>
using namespace std;

bool checkTimeFormat(string test);

//-----------------------------------------------------------------
// MyTime::MyTime(const string &time)
//
// Summary: Calculates new objects time in minutes from input string
//			Throws if format is faulty
//
// Parameters:	const string &time - 24h format time string (hh:mm)
//
// Returns:	-
//-----------------------------------------------------------------
MyTime::MyTime(const string &time)
{
	bool goodFormat;
	int hours = 0, minutes = 0;
	if (goodFormat = checkTimeFormat(time))
	{
		char delim;
		istringstream timeStream(time);
		timeStream >> hours >> delim >> minutes;

		m_time = hours * 60 + minutes;
	}
	
	if (!goodFormat || hours < 0 || minutes < 0 || minutes > 59)	//Let the user enter hours > 24 for now since its the only way to set time a day forward
	{
		ostringstream message;
		message << "MyTime: Faulty constructor argument: " << time;
		throw runtime_error(message.str());
	}
}

//-----------------------------------------------------------------
// std::ostream &operator<<(std::ostream &os, const MyTime &time)
//
// Summary: Feeds ostream with a 24h format representation of time
//
// Parameters:	ostream &os - stream to receive data
//				const MyTime &time - time object to be formated
//
// Returns:	ostream&
//-----------------------------------------------------------------
ostream &operator<<(ostream &os, const MyTime &time)
{
	int timeMinutes = time.getTime();
	timeMinutes = timeMinutes % (24 * 60);

	int hours = timeMinutes / 60;
	if (hours < 10)
		os << '0';
	os << hours << ':';

	int minutes = timeMinutes % 60;
	if (minutes < 10)
		os << '0';
	os << minutes;

	int day = 1 + time.getTime() / (24 * 60);
	if (day > 1)
		os << "(day " << day << ")";
	return os;
}

//-----------------------------------------------------------------
// istream &operator>>(istream &is, MyTime &time)
//
// Summary: Attempts to create a MyTime object from input in stream
//			Sets failbit on stream if format is faulty
//
// Parameters:	istream &is - user input stream
//				MyTime &time - time object to be initialized
//
// Returns:	istream&
//-----------------------------------------------------------------
istream &operator>>(istream &is, MyTime &time)
{
	string input;
	is >> input;

	int hours = 0, minutes = 0;
	if (checkTimeFormat(input))
	{
		char delim;
		istringstream timeStream(input);
		timeStream >> hours >> delim >> minutes;

		if (hours < 0 || minutes < 0 || minutes > 59)	//Let the user enter hours > 24 for now since its the only way to set time a day forward
			is.setstate(ios::failbit);

		if (is.good())
			time.m_time = hours * 60 + minutes;
	}
	else
		is.setstate(ios::failbit);

	return is;
}

//-----------------------------------------------------------------
// bool checkTimeFormat(string test)
//
// Summary: Makes sure that there are no invalid characters
//			according to 24h format and that both hours and minutes
//			are represented by 2 characters
//
// Parameters:	string test - string to evaluate
//
// Returns:	bool - true if format is ok
//-----------------------------------------------------------------
bool checkTimeFormat(string test)
{
	istringstream testStream(test);
	string hours, minutes;

	getline(testStream, hours, ':');
	testStream >> minutes;

	if (hours.length() == 2 && minutes.length() == 2 && hours.find_first_not_of("0123456789") == string::npos && minutes.find_first_not_of("0123456789") == string::npos)
	{
		return true;
	}
	else
		return false;
}