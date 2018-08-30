// File: SleepWagon.cpp
// Written by: Johan Lilja
// Date: 2015-10-25
// Last edited: 2015-11-01
// Course: DT060G OOP C++
// Assignment: Project Trains
#include "SleepWagon.h"
#include <istream>
#include <sstream>
#include <string>
using namespace std;

//-----------------------------------------------------------------
// SleepWagon::SleepWagon(const int id, std::istream &wagonData) : TrainWagon(id, SLEEP) 
//
// Summary: Reads wagon specific data
//
// Parameters:	const int id - The wagon ID
//				std::istream &wagonData - Data specific to the wagon type
//
// Returns:	-
//-----------------------------------------------------------------
SleepWagon::SleepWagon(const int id, std::istream &wagonData) : TrainWagon(id, SLEEP)
{
	wagonData >> m_beds;
}

//-----------------------------------------------------------------
// string SleepWagon::getInfo(const bool verbose) const
//
// Summary: Returns information about wagon
//
// Parameters:	const bool verbose - how much info that should be returned
//
// Returns:	string - wagon information
//-----------------------------------------------------------------
string SleepWagon::getInfo(const bool verbose) const
{
	ostringstream info;
	info << TrainWagon::getInfo(verbose);
	if (verbose)
		info << ", Beds: " << m_beds;
	return info.str();
}