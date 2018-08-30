// File: SeatWagon.h
// Written by: Johan Lilja
// Date: 2015-10-25
// Last edited: 2015-11-01
// Course: DT060G OOP C++
// Assignment: Project Trains
#ifndef SEATWAGON_H
#define SEATWAGON_H
#include "TrainWagon.h"

class SeatWagon : public TrainWagon
{
private:
	int m_seats;
	bool m_internet;
public:
	SeatWagon(const int id, std::istream &wagonData);
	virtual std::string getInfo(const bool verbose) const;
};

#endif