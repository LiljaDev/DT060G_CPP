// File: CloseCargoWagon.h
// Written by: Johan Lilja
// Date: 2015-10-25
// Last edited: 2015-11-01
// Course: DT060G OOP C++
// Assignment: Project Trains
#ifndef CLOSEDCARGOWAGON_H
#define CLOSEDCARGOWAGON_H
#include "TrainWagon.h"

class ClosedCargoWagon : public TrainWagon
{
private:
	int m_volume;
public:
	ClosedCargoWagon(const int id, std::istream &wagonData);
	virtual std::string getInfo(const bool verbose) const;
};

#endif