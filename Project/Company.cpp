// File: Company.cpp
// Written by: Johan Lilja
// Date: 2015-10-25
// Last edited: 2015-11-01
// Course: DT060G OOP C++
// Assignment: Project Trains
#include "Company.h"
#include "Constants.h"
#include "TrainStation.h"
#include "MyTime.h"
#include "WagonHistory.h"
#include "TrainWagon.h"
#include "SeatWagon.h"
#include "SleepWagon.h"
#include "OpenCargoWagon.h"
#include "ClosedCargoWagon.h"
#include "ElectricLocomotive.h"
#include "DieselLocomotive.h"

#include <fstream>
#include <sstream>
#include <memory>
#include <string>
#include <vector>
using namespace std;
using namespace constants;

//-----------------------------------------------------------------
// void Company::init()
//
// Summary: Initialize the company by reading data files containing the stations, wagons and trains
//
// Parameters:	-
//
// Returns:	-
//-----------------------------------------------------------------
void Company::init()
{
	loadStations();
	loadMap();
	loadTrains();
}

bool Company::getTrains(std::map<int, std::weak_ptr<const Train>> &trains) const
{ 
	trains.insert(m_trains.begin(), m_trains.end());
	return true; 
}

bool Company::getStations(std::vector<std::weak_ptr<const TrainStation>> &stations) const
{
	for (const auto station : m_stations)
	{
		stations.push_back(station.second);
	}
	
	return true;
}

weak_ptr<const Train> Company::getTrain(const int number) const
{
	weak_ptr<const Train> train;
	if (m_trains.find(number) != m_trains.end())
		train = m_trains.at(number);

	return train;
}

//-----------------------------------------------------------------
// string Company::getWagonLocation(const int id) const
//
// Summary: Gets information about a specific wagons location
//
// NOTE: Would have wanted to return pointer but did not plan for this
//		 compromise solution returns a string instead
//
// Parameters:	const int id - wagon to get location of
//
// Returns:	string - wagon location information (empty if not found)
//-----------------------------------------------------------------
string Company::getWagonLocation(const int id) const
{
	ostringstream location;

	for (const auto &station : m_stations)	//Search all stations for the wagon
	{
		multimap<WAGON_TYPE, std::weak_ptr<const TrainWagon>> wagons;
		station.second->getWagons(wagons);
		for (const auto &wagon : wagons)
		{
			const auto wagonPtr = wagon.second.lock();
			if (wagonPtr && wagonPtr->getID() == id)
			{
				location << station.second->getName();
				return location.str();
			}
		}
	}

	for (const auto &train : m_trains)	//Search all trains for the wagon
	{
		vector<weak_ptr<const TrainWagon>> wagons;
		train.second->getWagons(wagons);
		for (const auto &wagon : wagons)
		{
			const auto wagonPtr = wagon.lock();
			if (wagonPtr && wagonPtr->getID() == id)
			{
				location << "Train " << train.second->getNumber();
				return location.str();
			}
		}
	}
	
	return location.str();
}

//-----------------------------------------------------------------
// WagonHistory Company::getWagonHistory(const int id) const
//
// Summary: Gets a wagons travel history
//
// Parameters:	const int id - wagon to get history of
//
// Returns:	WagonHistory - object containing the wagons history
//-----------------------------------------------------------------
WagonHistory Company::getWagonHistory(const int id) const
{
	for (const auto &station : m_stations)	//Search all station for the wagon
	{
		multimap<WAGON_TYPE, std::weak_ptr<const TrainWagon>> wagons;
		station.second->getWagons(wagons);
		for (const auto &wagon : wagons)
		{
			const auto wagonPtr = wagon.second.lock();
			if (wagonPtr && wagonPtr->getID() == id)
			{
				return wagonPtr->getHistory();
			}
		}
	}

	for (const auto &train : m_trains)	//Search all trains for the wagon
	{
		vector<weak_ptr<const TrainWagon>> wagons; 
		train.second->getWagons(wagons);
		for (const auto &wagon : wagons)
		{
			const auto wagonPtr = wagon.lock();
			if (wagonPtr && wagonPtr->getID() == id)
			{
				return wagonPtr->getHistory();
			}
		}
	}

	WagonHistory emptyHistory;
	return emptyHistory;
}

//Returns the trains new state after attempting to assemble
TRAIN_STATE Company::assembleTrain(const int trainNumber)
{
	return m_trains.at(trainNumber)->assemble();
}

//-----------------------------------------------------------------
// void Company::delayTrain(const int trainNumber, const int delay)
//
// Summary: Delay a trains departure
//			This does not necessarily mean that the arrival is delayed
//
// Parameters:	const int trainNumber - Number of the train to delay
//				const int delay - The time of delay in minutes
//
// Returns:	-
//-----------------------------------------------------------------
void Company::delayTrain(const int trainNumber, const int delay)
{
	m_trains.at(trainNumber)->delay(delay);
}

//Delay the trains arrival by 'delay' number of minutes
void Company::delayTrainArrival(const int trainNumber, const int delay)
{
	m_trains.at(trainNumber)->delayArrival(delay);
}

//Tell train to go into READY state
void Company::readyTrain(const int trainNumber)
{
	m_trains.at(trainNumber)->readyUp();
}

//Tell train to go into RUN state
void Company::runTrain(const int trainNumber)
{
	m_trains.at(trainNumber)->run();
}

//Tell train to go into ARRIVE state
void Company::arriveTrain(const int trainNumber)
{
	m_trains.at(trainNumber)->arrive();
}

//Tell train to disassemble
void Company::disassembleTrain(const int trainNumber)
{
	m_trains.at(trainNumber)->disassemble();
}

//-----------------------------------------------------------------
// void Company::loadStations()
//
// Summary: Loads stations from file
//			Throws exceptions if file could not open or if parsing fails
//
// Parameters:	-
//
// Returns:	-
//-----------------------------------------------------------------
void Company::loadStations()
{
	ifstream inFile(STATIONS_FILENAME);
	if (!inFile.is_open())
		throw runtime_error("Station info file could not be opened");

	string tmp;
	while (getline(inFile, tmp))	//Read one line (all data regarding one station) at a time
	{
		istringstream stationSection(tmp);
		ios_base::iostate mask = ios::badbit;
		stationSection.exceptions(mask);

		//Get station name and add it to m_stations map
		string stationName;
		stationSection >> stationName;
		shared_ptr<TrainStation> station = make_shared<TrainStation>(stationName);
		m_stations[stationName] = station;
		
		while (getline(stationSection, tmp, STATIONS_DELIM))	//Read data regarding one wagon at a time
		{
			//Remove pesky '('
			auto skipTo = find(tmp.begin(), tmp.end(), '(');
			if (skipTo != tmp.end())
				tmp.erase(tmp.begin(), ++skipTo);	

			//Feed wagon data into stream
			istringstream wagonSection(tmp);
			ios_base::iostate mask = ios::badbit;
			wagonSection.exceptions(mask);

			//Parse id and type and create a new wagon based on it
			int id, type;
			wagonSection >> id;
			wagonSection >> type;
			shared_ptr<TrainWagon> wagon;
			switch (type)
			{
			case SEAT:
				wagon = make_shared<SeatWagon>(id, wagonSection);
				break;
			case SLEEP:
				wagon = make_shared<SleepWagon>(id, wagonSection);
				break;
			case OPEN_CARGO:
				wagon = make_shared<OpenCargoWagon>(id, wagonSection);
				break;
			case CLOSED_CARGO:
				wagon = make_shared<ClosedCargoWagon>(id, wagonSection);
				break;
			case EL_LOC:
				wagon = make_shared<ElectricLocomotive>(id, wagonSection);
				break;
			case DIESEL_LOC:
				wagon = make_shared<DieselLocomotive>(id, wagonSection);
				break;
			}

			//Add wagon to the station
			station->parkWagon(wagon);
		}
	}
}

//-----------------------------------------------------------------
// void Company::loadMap()
//
// Summary: Loads info about distances between stations from file
//			Throws exceptions if file could not open or if parsing fails
//
// Parameters:	-
//
// Returns:	-
//-----------------------------------------------------------------
void Company::loadMap()
{
	ifstream inFile(MAP_FILENAME);
	if (!inFile.is_open())
		throw runtime_error("Map file could not be opened");

	string tmp;
	while (getline(inFile, tmp))	//Read one line containing two stations and a distance
	{
		istringstream data(tmp);
		ios_base::iostate mask = ios::badbit;
		data.exceptions(mask);

		//Read station names and distance then inform both stations of this relation
		string firstStation, secondStation;
		int distance;
		data >> firstStation >> secondStation >> distance;
		m_stations.at(firstStation)->setDistance(secondStation, distance);
		m_stations.at(secondStation)->setDistance(firstStation, distance);
	}
}

//-----------------------------------------------------------------
// void Company::loadTrains()
//
// Summary: Loads info about trains from file
//			Throws exceptions if file could not open or if parsing fails
//
// Parameters:	-
//
// Returns:	-
//-----------------------------------------------------------------
void Company::loadTrains()
{
	ifstream inFile(TRAINS_FILENAME);
	if (!inFile.is_open())
		throw runtime_error("Train info file could not be opened");

	string tmp;
	while (getline(inFile, tmp))	//Read one line
	{
		istringstream trainData(tmp);
		ios_base::iostate mask = ios::badbit;
		trainData.exceptions(mask);

		int trainNumber, maxSpeed;
		string from, to, depText, arrText;
		vector<WAGON_TYPE> wagonStructure;

		//Parse train data
		trainData >> trainNumber;
		trainData >> from >> to >> depText >> arrText;
		trainData >> maxSpeed;

		//Read while stream is ok, number of wagons varies
		int tmpType;
		while (trainData >> tmpType)
		{
			if (tmpType < WAGON_TYPE_COUNT)
			{
				wagonStructure.push_back(static_cast<WAGON_TYPE>(tmpType));
			}
			else   //Throw range_error because wagon type is unknown
			{
				ostringstream message;
				message << "Unknown wagon type " << tmpType << " in " << TRAINS_FILENAME;
				throw range_error(message.str());
			}
		}

		MyTime depTime(depText);
		MyTime arrTime(arrText);

		if (m_stations.find(from) == m_stations.end() || m_stations.find(to) == m_stations.end())	//Throw if one of the stations doesnt exist
			throw out_of_range("Encountered unknown station");
		weak_ptr<TrainStation> fromStation = m_stations.at(from);
		weak_ptr<TrainStation> toStation = m_stations.at(to);
		m_trains[trainNumber] = make_shared<Train>(trainNumber, fromStation, toStation, depTime, arrTime, maxSpeed, wagonStructure);
	}
}