// File: SeatWagon.cpp
// Written by: Johan Lilja
// Date: 2015-10-25
// Last edited: 2015-11-01
// Course: DT060G OOP C++
// Assignment: Project Trains
#include "SeatWagon.h"
#include <string>
#include <sstream>
using namespace std;

//-----------------------------------------------------------------
// SeatWagon::SeatWagon(const int id, std::istream &wagonData)
//
// Summary: Reads wagon specific data
//
// Parameters:	const int id - The wagon ID
//				std::istream &wagonData - Data specific to the wagon type
//
// Returns:	-
//-----------------------------------------------------------------
SeatWagon::SeatWagon(const int id, std::istream &wagonData) : TrainWagon(id, SEAT)
{
	wagonData >> m_seats;
	wagonData >> m_internet;
}

//-----------------------------------------------------------------
// string SeatWagon::getInfo(const bool verbose) const
//
// Summary: Returns information about wagon
//
// Parameters:	const bool verbose - how much info that should be returned
//
// Returns:	string - wagon information
//-----------------------------------------------------------------
string SeatWagon::getInfo(const bool verbose) const
{
	ostringstream info;
	info << TrainWagon::getInfo(verbose);
	if (verbose)
	{
		info << ", Seats: " << m_seats <<
			", Internet connection: ";
		if (m_internet)
			info << "Yes";
		else
			info << "No";
	}
	return info.str();
}