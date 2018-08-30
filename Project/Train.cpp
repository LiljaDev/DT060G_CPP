// File: Train.cpp
// Written by: Johan Lilja
// Date: 2015-10-25
// Last edited: 2015-11-01
// Course: Objektorienterad programmering i C++
// Assignment: Project Trains
#include "Train.h"
#include "TrainStation.h"
#include "Locomotive.h"
#include <sstream>
#include <cmath>
#include <iomanip>
using namespace std;

Train::Train(const int number, std::weak_ptr<TrainStation> origin, std::weak_ptr<TrainStation> destination, 
	const MyTime &departure, const MyTime &arrival, const int maxSpeed, const std::vector<WAGON_TYPE> &requiredWagons) :
	m_number(number), m_origin(origin), m_destination(destination), m_departureTime(departure), 
	m_arrivalTime(arrival), m_departureDelay(0), m_arrivalDelay(0), m_maxSpeed(maxSpeed), m_averageSpeed(0), m_requiredWagons(requiredWagons), 
	m_state(NOT_ASSEMBLED), m_wagons(requiredWagons.size(), nullptr) { }


//-----------------------------------------------------------------
// string Train::getInfo(const bool verbose) const
//
// Summary: Returns information about train
//
// Parameters:	const bool verbose - how much info that should be returned
//
// Returns:	string - train information
//-----------------------------------------------------------------
string Train::getInfo(const bool verbose) const
{
	ostringstream out;
	shared_ptr<TrainStation> origin = m_origin.lock();
	shared_ptr<TrainStation> destination = m_destination.lock();

	if (origin && destination)
	{
		out << "Train[" << m_number << "]";	//Always display train number
		//Display additional info if it is requested (verbose)
		if (verbose){
			out << " from " << origin->getName() << " " << m_departureTime - m_departureDelay << "(" << m_departureTime <<
				") to " << destination->getName() << " " << m_arrivalTime - m_arrivalDelay << "(" << m_arrivalTime << ")" << " state: " << TRAIN_STATE_NAMES[m_state] << ", Max allowed speed: " << m_maxSpeed << " km/h";
			//Only display average speed if the train has actually traveled
			if (m_state == ARRIVED || m_state == FINISHED)
				out << ", Average speed: " << fixed << setprecision(1) << m_averageSpeed << " km/h" << endl;
			else
				out << endl;
		}
		else
			out << endl;
		//Get info for each connected wagon, verbose is passed on
		for (int i = 0; i < m_wagons.size(); ++i)
		{
			if (m_wagons[i])
			{
				out << m_wagons[i]->getInfo(verbose) << endl;
			}
			else
				out << TYPE_NAMES[m_requiredWagons[i]] << " (missing)" << endl;
		}
	}

	return out.str();
}

bool Train::getWagons(std::vector<std::weak_ptr<const TrainWagon>> &wagons) const
{
	wagons.assign(m_wagons.begin(), m_wagons.end());
	return true;
}

string Train::getOriginName() const
{
	auto origin = m_origin.lock();
	if (origin)
		return origin->getName();
	else
		return "unknown";
}

string Train::getDestinationName() const
{
	auto destination = m_destination.lock();
	if (destination)
		return destination->getName();
	else
		return "unknown";
}

//-----------------------------------------------------------------
// TRAIN_STATE Train::assemble()
//
// Summary: Attempt to assemble train from the available wagons at
//			train origin
//
// Parameters:	-
//
// Returns:	TRAIN_STATE - the trains state after attempting to assemble
//-----------------------------------------------------------------
TRAIN_STATE Train::assemble()
{
	bool success = true;
	auto station = m_origin.lock();

	for (int i = 0; i < m_wagons.size(); ++i)
	{
		if (!m_wagons[i])
		{
			shared_ptr<TrainWagon> commandeered = station->withdrawWagon(m_requiredWagons[i]);	//try to fetch wagon of correct type from station, might return nullptr
			if (commandeered)
			{
				m_wagons[i] = commandeered;
				m_wagons[i]->logTrain(m_number);
			}
			else
				success = false;	//Train will not be complete but we keep trying to fetch to give priority to the earlies arriving trains
		}
	}

	if (success)
		m_state = ASSEMBLED;
	else
		m_state = INCOMPLETE;

	station->addTrain(shared_from_this());	//Inform the station that this now counts as a live train
	return m_state;
}

//-----------------------------------------------------------------
// void Train::run()
//
// Summary: Changes state to RUNNING and removes the train from station
//
// Parameters:	-
//
// Returns: -
//-----------------------------------------------------------------
void Train::run()
{
	m_state = RUNNING;
	auto station = m_origin.lock();
	station->removeTrain(shared_from_this());
}

//-----------------------------------------------------------------
// void Train::arrive()
//
// Summary: Changes state to ARRIVED and calculates the trains average speed
//			also adds the train to destination station
//
// Parameters:	-
//
// Returns: -
//-----------------------------------------------------------------
void Train::arrive()
{
	m_state = ARRIVED;
	m_averageSpeed = getTravelDistance() / ((m_arrivalTime - m_departureTime).getTime() / 60.0);
	auto station = m_destination.lock();
	station->addTrain(shared_from_this());
}

//-----------------------------------------------------------------
// void Train::disassemble()
//
// Summary: Changes state to FINISHED and places the wagons at the destination
//			Also removes the train from the destination, the train no longer physically exists
//
// Parameters:	-
//
// Returns: -
//-----------------------------------------------------------------
void Train::disassemble()
{
	m_state = FINISHED;
	shared_ptr<TrainStation> station = m_destination.lock();
	for (int i = 0; i < m_wagons.size(); ++i)
	{
		station->parkWagon(m_wagons[i]);
		/*m_wagons[i]->logStation(station->getName());*/
		m_wagons[i].reset();
	}

	station->removeTrain(shared_from_this());
}

//-----------------------------------------------------------------
// void Train::delay(const int addedTime)
//
// Summary: Adds a delay time to departure and calculates if arrival needs
//			to be delayed as well based on the trains maximum allowed speed
//
// Parameters:	const int addedTime - delay in minutes
//
// Returns: -
//-----------------------------------------------------------------
void Train::delay(const int addedTime)
{
	m_departureTime += addedTime;
	m_departureDelay += addedTime;
	int maxSpeed = getMaxSpeed();
	double maxDistance = maxSpeed * ((m_arrivalTime - m_departureTime).getTime() / 60.0);
	if (maxDistance < getTravelDistance())
	{
		double missingDistance = getTravelDistance() - maxDistance;
		int missingTime = ceil((missingDistance / maxSpeed) * 60);
		delayArrival(missingTime);
	}
}

//Add delay to arival
void Train::delayArrival(const int addedTime)
{
	m_arrivalTime += addedTime;
	m_arrivalDelay += addedTime;
}

//Get the distance from origin to destination
int Train::getTravelDistance() const
{
	shared_ptr<TrainStation> origin = m_origin.lock();
	shared_ptr<TrainStation> destination = m_destination.lock();
	if (origin && destination)
		return origin->getDistance(destination->getName());
	else
		return -1;
}

//-----------------------------------------------------------------
// int Train::getMaxSpeed() const
//
// Summary: Calculates the trains true max speed, meaning it takes the
//			locomotives into consideration on top of the trains own 
//			max speed
//
// Parameters:
//
// Returns: int - max speed of the train
//-----------------------------------------------------------------
int Train::getMaxSpeed() const
{
	int maxSpeed = m_maxSpeed;
	for (const auto &wagon : m_wagons)
	{
		if (wagon)
		{
			int tmp = wagon->getMaxSpeed();
			if (tmp > 0 && tmp < maxSpeed)
				maxSpeed = tmp;
		}
	}
	return maxSpeed;
}