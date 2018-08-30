#ifndef COMPANY_H
#define COMPANY_H
#include "TrainWagon.h"
#include "TrainStation.h"
#include "Train.h"
#include <map>
// File: Company.h
// Written by: Johan Lilja
// Date: 2015-10-25
// Last edited: 2015-11-01
// Course: DT060G OOP C++
// Assignment: Project Trains
#include <memory>
#include <string>
class Company
{
private:
	std::map<int, std::shared_ptr<Train>> m_trains;
	std::map<std::string, std::shared_ptr<TrainStation>> m_stations;

	void loadStations();
	void loadMap();
	void loadTrains();

public:
	void init();
	bool getTrains(std::map<int, std::weak_ptr<const Train>> &trains) const;
	bool getStations(std::vector<std::weak_ptr<const TrainStation>> &stations) const;
	std::weak_ptr<const Train> getTrain(const int number) const;
	std::string getWagonLocation(const int id) const;
	WagonHistory getWagonHistory(const int id) const;
	TRAIN_STATE assembleTrain(const int trainNumber);
	void runTrain(const int trainNumber);
	void arriveTrain(const int trainNumber);
	void disassembleTrain(const int trainNumber);
	void delayTrain(const int trainNumber, const int delay);
	void delayTrainArrival(const int trainNumber, const int delay);
	void readyTrain(const int trainNumber);
};

#endif