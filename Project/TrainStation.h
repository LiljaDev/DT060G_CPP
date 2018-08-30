// File: TrainStation.h
// Written by: Johan Lilja
// Date: 2015-10-25
// Last edited: 2015-11-01
// Course: DT060G OOP C++
// Assignment: Project Trains
#ifndef TRAINSTATION_H
#define TRAINSTATION_H
#include "Train.h"
#include "TrainWagon.h"
#include <memory>
#include <vector>
#include <string>
#include <map>
class TrainStation
{
private:
	std::string m_stationName;
	//std::map<int, std::weak_ptr<Train>> m_trains;
	std::vector<std::weak_ptr<const Train>> m_trains;
	std::multimap<WAGON_TYPE, std::shared_ptr<TrainWagon>> m_availableWagons;
	std::map<std::string, int> m_distances;
public:
	TrainStation(const std::string &name) : m_stationName(name) {}
	void parkWagon(std::shared_ptr<TrainWagon>);
	std::shared_ptr<TrainWagon> withdrawWagon(WAGON_TYPE);
	bool getWagons(std::multimap<WAGON_TYPE, std::weak_ptr<const TrainWagon>> &wagons) const;
	bool getTrains(std::vector<std::weak_ptr<const Train>> &trains) const;
	void setDistance(const std::string &toStation, const int distance);
	int getDistance(const std::string &to) const;
	std::string getName() const { return m_stationName; }
	void addTrain(std::weak_ptr<const Train>);
	void removeTrain(std::weak_ptr<const Train>);
};

#endif