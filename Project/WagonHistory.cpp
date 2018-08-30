// File: Wagonhistory.cpp
// Written by: Johan Lilja
// Date: 2015-10-25
// Last edited: 2015-11-01
// Course: DT060G OOP C++
// Assignment: Project Trains
#include "WagonHistory.h"
#include <string>
#include <sstream>
using namespace std;

//-----------------------------------------------------------------
// void WagonHistory::logTrain(const int id)
//
// Summary: Log a connection to a train
//
// Parameters:	const int id - The train ID
//
// Returns:	-
//-----------------------------------------------------------------
void WagonHistory::logTrain(const int id)
{
	ostringstream log;
	log << "Wagon connected to train " << id;
	m_logs.push_back(log.str());
}

//-----------------------------------------------------------------
// void WagonHistory::logStation(const string stationName)
//
// Summary: Log a connection to a station
//
// Parameters:	const string stationName - The station name
//
// Returns:	-
//-----------------------------------------------------------------
void WagonHistory::logStation(const string stationName)
{
	ostringstream log;
	log << "Wagon is available at station " << stationName;
	m_logs.push_back(log.str());
}