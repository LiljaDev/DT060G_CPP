// File: DieselLocomotive.h
// Written by: Johan Lilja
// Date: 2015-10-25
// Last edited: 2015-11-01
// Course: DT060G OOP C++
// Assignment: Project Trains
#ifndef DIESELLOCOMOTIVE_H
#define DIESELLOCOMOTIVE_H
#include "Locomotive.h"

class DieselLocomotive : public Locomotive
{
private:
	int m_fuelUsage;
public:
	DieselLocomotive(const int id, std::istream &wagonData);
	virtual std::string getInfo(const bool verbose) const;
};

#endif