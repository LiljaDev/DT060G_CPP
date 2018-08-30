// File: Locomotive.h
// Written by: Johan Lilja
// Date: 2015-10-25
// Last edited: 2015-11-01
// Course: DT060G OOP C++
// Assignment: Project Trains
#ifndef LOCOMOTIVE_H
#define LOCOMOTIVE_H
#include "TrainWagon.h"
#include <sstream>

class Locomotive : public TrainWagon
{
protected:
	int m_maxSpeed;
public:
	Locomotive(const int id, const WAGON_TYPE type) : TrainWagon(id, type) {}
	virtual ~Locomotive() = 0 {};	//Make abstract without enforcing derived classes to override anything
	virtual int getMaxSpeed() const { return m_maxSpeed; }
	virtual std::string getInfo(const bool verbose) const;
};

#endif