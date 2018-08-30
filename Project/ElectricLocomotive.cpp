// File: ElectricLocomotive.cpp
// Written by: Johan Lilja
// Date: 2015-10-25
// Last edited: 2015-11-01
// Course: DT060G OOP C++
// Assignment: Project Trains
#include "ElectricLocomotive.h"
using namespace std;

//-----------------------------------------------------------------
// ElectricLocomotive::ElectricLocomotive(const int id, std::istream &wagonData)
//
// Summary: Reads wagon specific data
//
// Parameters:	const int id - The wagon ID
//				std::istream &wagonData - Data specific to the wagon type
//
// Returns:	-
//-----------------------------------------------------------------
ElectricLocomotive::ElectricLocomotive(const int id, std::istream &wagonData) : Locomotive(id, EL_LOC)
{
	wagonData >> m_maxSpeed;
	wagonData >> m_effect;
}

//-----------------------------------------------------------------
// string ElectricLocomotive::getInfo(const bool verbose) const
//
// Summary: Returns information about wagon
//
// Parameters:	const bool verbose - how much info that should be returned
//
// Returns:	string - wagon information
//-----------------------------------------------------------------
string ElectricLocomotive::getInfo(const bool verbose) const
{
	ostringstream info;
	info << Locomotive::getInfo(verbose);
	if (verbose)
		info << ", Effect: " << m_effect << " kW";

	return info.str();
}