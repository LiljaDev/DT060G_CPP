// File: ElectricLocomotive.h
// Written by: Johan Lilja
// Date: 2015-10-25
// Last edited: 2015-11-01
// Course: DT060G OOP C++
// Assignment: Project Trains
#ifndef ELECTRICLOCOMOTIVE_H
#define ELECTRICLOCOMOTIVE_H
#include "Locomotive.h"

class ElectricLocomotive : public Locomotive
{
private:
	int m_effect;
public:
	ElectricLocomotive(const int id, std::istream &wagonData);
	virtual std::string getInfo(const bool verbose) const;
};

#endif