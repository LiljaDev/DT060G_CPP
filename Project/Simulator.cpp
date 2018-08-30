// File: Simulator.cpp
// Written by: Johan Lilja
// Date: 2015-10-25
// Last edited: 2015-11-01
// Course: DT060G OOP C++
// Assignment: Project Trains
#include "Simulator.h"
#include "Event.h"
#include "Company.h"
#include "UI.h"
#include <iostream>
#include <map>
#include <vector>
using namespace std;


//-----------------------------------------------------------------
// void Simulator::run()
//
// Summary: Initializes the simulator with first series of events by calling init()
//			Lets user customize settings by calling mainMenu()
//			Runs the simulation and displays simMenu() between every step
//			Finally calculates and displays statistics by calling statistics()
//
// Parameters:	-
//
// Returns:	-
//-----------------------------------------------------------------
void Simulator::run()
{
	init();
	bool start = mainMenu();
	if (!start)
		return;

	MyTime previousTime = m_currentTime = m_startTime;
	while (!m_eventQueue.empty())
	{

		Event * nextEvent = m_eventQueue.top();
		if (m_intervalMode)	//If user is stepping simulation manually
		{
			m_currentTime += m_interval;	//Go forward in time based on interval size

			//Keep going as long as there are events and their times are before the current time
			//But also keep going until the queue is empty in case the next event takes place after the feedback stop time
			while (!m_eventQueue.empty() && (m_currentTime >= nextEvent->getTime() || nextEvent->getTime() > m_visualStopTime))
			{
				m_eventQueue.pop();
				nextEvent->processEvent();
				delete nextEvent;
				if (!m_eventQueue.empty())
					nextEvent = m_eventQueue.top();
			}
		}
		else  //If user chose auto mode, step one event at a time
		{
			m_currentTime = nextEvent->getTime();	//Go forward in time to the next event
			if (m_currentTime >= m_startTime && m_currentTime <= m_visualStopTime)	//Process one event if the current time is within feedback range
			{
				m_eventQueue.pop();
				nextEvent->processEvent();
				delete nextEvent;
			}
			else  //Otherwise keep going until the time gets within feedback range or until the queue is empty
			{
				do
				{
					m_eventQueue.pop();
					nextEvent->processEvent();
					delete nextEvent;
					if (!m_eventQueue.empty())
					{
						nextEvent = m_eventQueue.top();
						m_currentTime = nextEvent->getTime();
					}
				} while (!m_eventQueue.empty() && (m_currentTime < m_startTime || m_currentTime > m_visualStopTime));
			}
		}

		m_ui->displayEventLogs(m_currentTime - (m_currentTime - previousTime), m_currentTime, m_visualStopTime, m_eventLogs);	//Display what took place during this timestep
		m_eventLogs.clear();

		//present user with sim menu
		simMenu();
		m_ui->clearScreen();
		previousTime = m_currentTime;
	}

	//Show statistics
	statistics();
	m_ui->waitForKeyboard();
}

//-----------------------------------------------------------------
// bool Simulator::init()
//
// Summary: Fills event queue with assemble events for each train
//			available from company
//
// Parameters:	-
//
// Returns:	-
//-----------------------------------------------------------------
void Simulator::init()
{
	map<int, std::weak_ptr<const Train>> trains; 
	m_company->getTrains(trains);
	for (const auto &train : trains)
	{
		auto trainPtr = train.second.lock();
		if (trainPtr)
		{
			scheduleEvent(new AssembleEvent(this, m_company, trainPtr->getDeparture() - 30, train.second));
			trainPtr->getDeparture();
		}
	}
}

//-----------------------------------------------------------------
// bool Simulator::mainMenu()
//
// Summary: Lets user customize simulation settings
//
// Parameters:	-
//
// Returns:	bool - true if user selected to start simulation
//-----------------------------------------------------------------
bool Simulator::mainMenu()
{
	bool done = false;
	int choice = 0;

	do
	{
		//get valid choice and act upon it
		choice = m_ui->mainMenu(m_startTime, m_visualStopTime, m_interval, m_intervalMode);
		switch (choice)
		{
		case 1:
			done = true;
			m_ui->clearScreen();
			break;
		case 2:
			m_ui->inputTimes(m_startTime, m_visualStopTime);
			break;
		case 3:
			m_interval = m_ui->inputInt("Select interval size: ", 1);
			break;
		case 4:
			m_intervalMode = !m_intervalMode;
			break;
		case 5:
			m_ui->toggleVerbose();
			break;
		case 6:
			return false;
			break;
		}
	} while (!done);

	return true;
}

//-----------------------------------------------------------------
// void Simulator::simMenu()
//
// Summary: Lets user browse simulation data before advancing the
//			simulation
//
// Parameters:	-
//
// Returns:	-
//-----------------------------------------------------------------
void Simulator::simMenu() const
{
	bool done = false;
	int choice = 0;

	do
	{
		//get valid menu choice and act upon it
		choice = m_ui->simMenu();
		switch (choice)
		{
		case 1:
			done = true;
			break;
		case 2:
			{
				  vector<weak_ptr<const Train>> schedule;
				  getSortedTrains(schedule);
				  m_ui->displaySchedule(schedule);
			}
			m_ui->waitForKeyboard();
			m_ui->clearScreen();
			break;
		case 3:
			m_ui->displayTrainInfo(m_company->getTrain(m_ui->inputInt("Train number: ")));
			m_ui->waitForKeyboard();
			m_ui->clearScreen();
			break;
		case 4:
			{
				  int id = m_ui->inputInt("Wagon id: ");
				  string location = m_company->getWagonLocation(id);
				  m_ui->displayWagonLocation(id, location);
			}
			m_ui->waitForKeyboard();
			m_ui->clearScreen();
			break;
		case 5:
			{
				  int id = m_ui->inputInt("Wagon id: ");
				  m_ui->displayWagonHistory(id, m_company->getWagonHistory(id));
			}
			m_ui->waitForKeyboard();
			m_ui->clearScreen();
			break;
		case 6:
			{
				  std::vector<std::weak_ptr<const TrainStation>> stations;
				  m_company->getStations(stations);
				  m_ui->displayStationInfo(stations);
			}
			
			m_ui->waitForKeyboard();
			m_ui->clearScreen();
			break;
		}
	} while (!done);
}

//-----------------------------------------------------------------
// vector<weak_ptr<const Train>> Simulator::getSortedTrains()
//
// Summary: Gets trains from company and sorts them into a vector by departure time
//
// Parameters:	-
//
// Returns:	vector<weak_ptr<const Train>> - Vector of trains sorted by departure time
//-----------------------------------------------------------------
void Simulator::getSortedTrains(vector<weak_ptr<const Train>> &schedule) const
{
	map<int, weak_ptr<const Train>> trains;
	m_company->getTrains(trains);

	for (const auto &train : trains){
		auto trainPtr = train.second.lock();
		if (trainPtr)
		{
			schedule.push_back(train.second);
		}
	}
	sort(schedule.begin(), schedule.end(), [](const weak_ptr<const Train> &lhs, const weak_ptr<const Train> &rhs) {
		auto lhsPtr = lhs.lock(), rhsPtr = rhs.lock();
		return lhsPtr->getDeparture() < rhsPtr->getDeparture();
	});
}

//-----------------------------------------------------------------
// void Simulator::statistics() const
//
// Summary: Calculates statistics and displays them by calling ui->displayStatistics()
//
// Parameters:	-
//
// Returns: -
//-----------------------------------------------------------------
void Simulator::statistics() const
{
	//Get the trains from company
	map<int, std::weak_ptr<const Train>> trains;
	m_company->getTrains(trains);

	//Some data for statistics
	vector<shared_ptr<const Train>> stuckTrains;	//Trains that never left the station
	vector<shared_ptr<const Train>> delayedTrains;	//Trains that were delayed
	MyTime totalDelay(0);
	MyTime lastTrainTime(0);	//When the last train arrived (NOT when it was finished)

	for (const auto train : trains)
	{
		auto trainPtr = train.second.lock();
		if (trainPtr->getState() != FINISHED)	//If train is not FINISHED it means that it never left the station
		{
			stuckTrains.push_back(trainPtr);
		}
		else if (trainPtr->getArrivalDelay() > 0)	// if the train was delayed
		{
			delayedTrains.push_back(trainPtr);	//keep track of all delayed trains (we do not care about departure delay) 
			totalDelay += trainPtr->getArrivalDelay().getTime();	//add the delay to total delay
			lastTrainTime = max(lastTrainTime, trainPtr->getArrival());
		}
	}
	int numSuccessful = trains.size() - delayedTrains.size() - stuckTrains.size();	//The number of trains that successfully arrived in time

	m_ui->displayStatistics(delayedTrains, stuckTrains, numSuccessful, totalDelay, lastTrainTime);	//Output statistics
}

//Pushes a new event into the queue
void Simulator::scheduleEvent(Event *newEvent)
{
	m_eventQueue.push(newEvent);
}