// File: Locomotive.cpp
// Written by: Johan Lilja
// Date: 2015-10-25
// Last edited: 2015-11-01
// Course: DT060G OOP C++
// Assignment: Project Trains
#include "Locomotive.h"
using namespace std;

//-----------------------------------------------------------------
// string Locomotive::getInfo(const bool verbose) const
//
// Summary: Returns information about wagon
//
// Parameters:	const bool verbose - how much info that should be returned
//
// Returns:	string - wagon information
//-----------------------------------------------------------------
string Locomotive::getInfo(const bool verbose) const
{
	ostringstream info;
	info << TrainWagon::getInfo(verbose);
	if (verbose)
		info << ", Max speed: " << m_maxSpeed << " km/h";

	return info.str();
}