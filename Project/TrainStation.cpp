// File: TrainStation.cpp
// Written by: Johan Lilja
// Date: 2015-10-25
// Last edited: 2015-11-01
// Course: DT060G OOP C++
// Assignment: Project Trains
#include "TrainWagon.h"
#include "TrainStation.h"
#include <algorithm>
using namespace std;

//Puts a wagon into the pool of available wagons
void TrainStation::parkWagon(shared_ptr<TrainWagon> wagon)
{
	m_availableWagons.insert(pair<WAGON_TYPE, shared_ptr<TrainWagon>>(wagon->getType(), wagon));
	wagon->logStation(m_stationName);
}

//Get vector of the trains that are currently on the station
bool TrainStation::getTrains(std::vector<std::weak_ptr<const Train>> &trains) const
{
	trains.assign(m_trains.begin(), m_trains.end());
	return true;
}

//Get map of the wagons that are currently available at the station
bool TrainStation::getWagons(std::multimap<WAGON_TYPE, std::weak_ptr<const TrainWagon>> &wagons) const
{
	wagons.insert(m_availableWagons.begin(), m_availableWagons.end());
	return true;
}

//Set distance to another station
void TrainStation::setDistance(const std::string &toStation, const int distance)
{
	m_distances[toStation] = distance;
}

//-----------------------------------------------------------------
// std::shared_ptr<TrainWagon> TrainStation::withdrawWagon(WAGON_TYPE type)
//
// Summary: Get a wagon from the stations wagon pool, if an acceptable wagon
//			is found it is also removed from the pool
//
// Parameters: WAGON_TYPE type - The requested type of wagon
//
// Returns: std::shared_ptr<TrainWagon> - pointer to the requested wagon
//-----------------------------------------------------------------
std::shared_ptr<TrainWagon> TrainStation::withdrawWagon(WAGON_TYPE type)
{
	shared_ptr<TrainWagon> chosenWagon(nullptr);
	pair <multimap<WAGON_TYPE, shared_ptr<TrainWagon>>::iterator, multimap<WAGON_TYPE, shared_ptr<TrainWagon>>::iterator> range;
	range = m_availableWagons.equal_range(type);
	
	if (range.first != range.second)	//If there's atleast one wagon of type 'type' in map
	{
		auto chosenIt = range.first;
		for (auto it = range.first; it != range.second; ++it)	//Iterate the range
		{
			if (it->second->getID() < chosenIt->second->getID())	//keep selecting the wagon with the lower ID
				chosenIt = it;
		}
		chosenWagon = chosenIt->second;
		m_availableWagons.erase(chosenIt);	//remove wagon from pool
	}

	return chosenWagon;
}

//add a train to the station
void TrainStation::addTrain(std::weak_ptr<const Train> train)
{
	m_trains.push_back(train);
}

//remove a train from the station
void TrainStation::removeTrain(std::weak_ptr<const Train> train)
{
	auto trainPtr = train.lock();
	if (trainPtr)
	{
		auto it = find_if(m_trains.begin(), m_trains.end(), [&trainPtr](const std::weak_ptr<const Train> &lhs){
			auto lhsPtr = lhs.lock();
			if (lhsPtr)
				return lhsPtr->getNumber() == trainPtr->getNumber();
			else
				return false; });
		if (it != m_trains.end())
			m_trains.erase(it);
	}
}

int TrainStation::getDistance(const std::string &to) const
{
	return m_distances.at(to);
}