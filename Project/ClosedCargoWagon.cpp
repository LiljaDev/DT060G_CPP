// File: ClosedCargoWagon.cpp
// Written by: Johan Lilja
// Date: 2015-10-25
// Last edited: 2015-11-01
// Course: DT060G OOP C++
// Assignment: Project Trains
#include "ClosedCargoWagon.h"
using namespace std;

//-----------------------------------------------------------------
// ClosedCargoWagon::ClosedCargoWagon(const int id, std::istream &wagonData)
//
// Summary: Reads wagon specific data
//
// Parameters:	const int id - The wagon ID
//				std::istream &wagonData - Data specific to the wagon type
//
// Returns:	-
//-----------------------------------------------------------------
ClosedCargoWagon::ClosedCargoWagon(const int id, std::istream &wagonData) : TrainWagon(id, CLOSED_CARGO)
{
	wagonData >> m_volume;
}

//-----------------------------------------------------------------
// string ClosedCargoWagon::getInfo(const bool verbose) const
//
// Summary: Returns information about wagon
//
// Parameters:	const bool verbose - how much info that should be returned
//
// Returns:	string - wagon information
//-----------------------------------------------------------------
string ClosedCargoWagon::getInfo(const bool verbose) const
{
	ostringstream info;
	info << TrainWagon::getInfo(verbose);
	if (verbose)
		info << ", Cargo volume: " << m_volume << " m^3";

	return info.str();
}