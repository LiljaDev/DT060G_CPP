// File: WagonHistory.h
// Written by: Johan Lilja
// Date: 2015-10-25
// Last edited: 2015-11-01
// Course: DT060G OOP C++
// Assignment: Project Trains
#ifndef WAGONHISTORY_H
#define WAGONHISTORY_H
#include <vector>
#include <string>
class WagonHistory
{
private:
	std::vector<const std::string> m_logs;
public:
	const std::vector<const std::string> &getLogs() const { return m_logs; }	//Get logs
	void logTrain(const int id);	//Log a connection to a train
	void logStation(const std::string stationName);	//Log a connection to a station
};

#endif