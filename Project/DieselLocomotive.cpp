// File: DieselLocomotive.cpp
// Written by: Johan Lilja
// Date: 2015-10-25
// Last edited: 2015-11-01
// Course: DT060G OOP C++
// Assignment: Project Trains
#include "DieselLocomotive.h"
using namespace std;

//-----------------------------------------------------------------
// DieselLocomotive::DieselLocomotive(const int id, std::istream &wagonData)
//
// Summary: Reads wagon specific data
//
// Parameters:	const int id - The wagon ID
//				std::istream &wagonData - Data specific to the wagon type
//
// Returns:	-
//-----------------------------------------------------------------
DieselLocomotive::DieselLocomotive(const int id, std::istream &wagonData) : Locomotive(id, DIESEL_LOC) 
{
	wagonData >> m_maxSpeed;
	wagonData >> m_fuelUsage;
}

//-----------------------------------------------------------------
// string DieselLocomotive::getInfo(const bool verbose) const
//
// Summary: Returns information about wagon
//
// Parameters:	const bool verbose - how much info that should be returned
//
// Returns:	string - wagon information
//-----------------------------------------------------------------
string DieselLocomotive::getInfo(const bool verbose) const
{
	ostringstream info;
	info << Locomotive::getInfo(verbose);
	if (verbose)
		info << ", Fuel usage: " << m_fuelUsage << " l/h";

	return info.str();
}