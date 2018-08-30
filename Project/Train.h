// File: Train.h
// Written by: Johan Lilja
// Date: 2015-10-25
// Last edited: 2015-11-01
// Course: DT060G OOP C++
// Assignment: Project Trains
#ifndef TRAIN_H
#define TRAIN_H
#include "TrainWagon.h"
#include "MyTime.h"
#include <vector>
#include <memory>
#include <sstream>
enum TRAIN_STATE{ NOT_ASSEMBLED, INCOMPLETE, ASSEMBLED, READY, RUNNING, ARRIVED, FINISHED };
const std::vector<std::string> TRAIN_STATE_NAMES = { "NOT_ASSEMBLED", "INCOMPLETE", "ASSEMBLED", "READY", "RUNNING", "ARRIVED", "FINISHED" };

class TrainStation;
class Train : public std::enable_shared_from_this<Train>
{
public:
	Train(const int number, std::weak_ptr<TrainStation> origin, std::weak_ptr<TrainStation> destination, const MyTime &departure, const MyTime &arrival, const int maxSpeed, const std::vector<WAGON_TYPE> &requiredWagons);
	std::string getInfo(const bool verbose) const;
	TRAIN_STATE getState() const { return m_state; }
	MyTime getDeparture() const { return m_departureTime; }
	MyTime getArrival() const { return m_arrivalTime; }
	MyTime getDepartureDelay() const { return m_departureDelay; }
	MyTime getArrivalDelay() const { return m_arrivalDelay; }
	int getNumber() const { return m_number; }
	int getMaxSpeed() const;
	int getTravelDistance() const;
	std::string	getOriginName() const;
	std::string	getDestinationName() const;
	bool getWagons(std::vector<std::weak_ptr<const TrainWagon>> &wagons) const;
	TRAIN_STATE assemble();
	void delay(const int addedTime);
	void delayArrival(const int addedTime);
	void readyUp() { m_state = READY; }
	void run();
	void arrive();
	void disassemble();

private:
	const int m_number;
	const int m_maxSpeed;
	double m_averageSpeed;
	std::vector<WAGON_TYPE> m_requiredWagons;
	std::weak_ptr<TrainStation> m_origin;
	std::weak_ptr<TrainStation> m_destination;
	TRAIN_STATE m_state;
	MyTime m_departureTime;
	MyTime m_departureDelay;
	MyTime m_arrivalTime;
	MyTime m_arrivalDelay;
	std::vector<std::shared_ptr<TrainWagon>> m_wagons;
};

#endif