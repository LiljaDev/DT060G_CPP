// File: OpenCargoWagon.cpp
// Written by: Johan Lilja
// Date: 2015-10-25
// Last edited: 2015-11-01
// Course: DT060G OOP C++
// Assignment: Project Trains
#include "OpenCargoWagon.h"
using namespace std;

//-----------------------------------------------------------------
// OpenCargoWagon::OpenCargoWagon(const int id, std::istream &wagonData)
//
// Summary: Reads wagon specific data
//
// Parameters:	const int id - The wagon ID
//				std::istream &wagonData - Data specific to the wagon type
//
// Returns:	-
//-----------------------------------------------------------------
OpenCargoWagon::OpenCargoWagon(const int id, std::istream &wagonData) : TrainWagon(id, OPEN_CARGO) 
{
	wagonData >> m_capacity;
	wagonData >> m_area;
}

//-----------------------------------------------------------------
// string OpenCargoWagon::getInfo(const bool verbose) const
//
// Summary: Returns information about wagon
//
// Parameters:	const bool verbose - how much info that should be returned
//
// Returns:	string - wagon information
//-----------------------------------------------------------------
string OpenCargoWagon::getInfo(const bool verbose) const
{
	ostringstream info;
	info << TrainWagon::getInfo(verbose);
	if (verbose)
	{
		info << ", Cargo capacity: " << m_capacity << " ton" <<
			", Cargo area: " << m_area << " m^2";
	}

	return info.str();
}