// File: TrainWagon.cpp
// Written by: Johan Lilja
// Date: 2015-10-25
// Last edited: 2015-11-01
// Course: DT060G OOP C++
// Assignment: Project Trains
#include "TrainWagon.h"
#include <sstream>
#include <string>
using namespace std;

//-----------------------------------------------------------------
// string TrainWagon::getInfo(const bool verbose) const
//
// Summary: Returns information about wagon
//
// Parameters:	const bool verbose - how much info that should be returned
//
// Returns:	string - wagon information
//-----------------------------------------------------------------
string TrainWagon::getInfo(const bool verbose) const
{
	ostringstream info;
	info << "[" << TYPE_NAMES[m_TYPE] << "] ID: " << m_ID;
	return info.str();
}