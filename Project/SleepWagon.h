// File: SleepWagon.h
// Written by: Johan Lilja
// Date: 2015-10-25
// Last edited: 2015-11-01
// Course: DT060G OOP C++
// Assignment: Project Trains
#ifndef SLEEPWAGON_H
#define SLEEPWAGON_H
#include "TrainWagon.h"

class SleepWagon : public TrainWagon
{
private:
	int m_beds;
public:
	SleepWagon(const int id, std::istream &wagonData);
	virtual std::string getInfo(const bool verbose) const;
};

#endif