// File: TrainWagon.h
// Written by: Johan Lilja
// Date: 2015-10-25
// Last edited: 2015-11-01
// Course: DT060G OOP C++
// Assignment: Project Trains
#ifndef TRAINWAGON_H
#define TRAINWAGON_H
#include "WagonHistory.h"
#include <string>
#include <vector>
#include <sstream>

enum WAGON_TYPE{SEAT, SLEEP, OPEN_CARGO, CLOSED_CARGO, EL_LOC, DIESEL_LOC};
const std::vector<std::string> TYPE_NAMES = { "Seat wagon", "Sleep wagon", "Open cargo wagon", "Closed cargo wagon", "Electric locomotive", "Diesel locomotive" };

class TrainWagon
{
private:
	const int m_ID;
	const WAGON_TYPE m_TYPE;
	WagonHistory m_history;
public:
	TrainWagon(const int id, const WAGON_TYPE type) : m_ID(id), m_TYPE(type) {}
	virtual ~TrainWagon() = 0 {};	//Make abstract without enforcing derived classes to override anything
	int getID() const { return m_ID; }
	WAGON_TYPE getType() const { return m_TYPE; }
	virtual std::string getInfo(const bool verbose) const;
	virtual int getMaxSpeed() const { return -1; }
	void logTrain(const int id) { m_history.logTrain(id); }
	void logStation(const std::string stationName){ m_history.logStation(stationName); }
	const WagonHistory &getHistory() const { return m_history; }
};


#endif