// File: OpenCargoLocomotive.h
// Written by: Johan Lilja
// Date: 2015-10-25
// Last edited: 2015-11-01
// Course: DT060G OOP C++
// Assignment: Project Trains
#ifndef OPENCARGOWAGON_H
#define OPENCARGOWAGON_H
#include "TrainWagon.h"

class OpenCargoWagon : public TrainWagon
{
private:
	int m_capacity;
	int m_area;
public:
	OpenCargoWagon(const int id, std::istream &wagonData);
	virtual std::string getInfo(const bool verbose) const;
};

#endif