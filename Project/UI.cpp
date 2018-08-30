// File: UI.cpp
// Written by: Johan Lilja
// Date: 2015-10-25
// Last edited: 2015-11-01
// Course: DT060G OOP C++
// Assignment: Project Trains
#include "UI.h"
#include "TrainStation.h"
#include "Constants.h"
#include <iostream>
#include <limits>
#include <vector>
#include "Train.h"
#include "SleepWagon.h"
using namespace std;

//-----------------------------------------------------------------
// int UI::mainMenu(const MyTime &startTime, const MyTime &stopTime, const int interval, const bool intervalMode)
//
// Summary:	Displays main menu and lets the user enter a valid choice
//
// Parameters:  const MyTime &startTime - current visual feedback start time
//				const MyTime &stopTime - current visual feedback stop time
//				const int interval - current simulation interval size
//				const bool intervalMode - if interval mode is currently selected
//
// Returns:	int - menu choice
//-----------------------------------------------------------------
int UI::mainMenu(const MyTime &startTime, const MyTime &stopTime, const int interval, const bool intervalMode)
{
	system("cls");
	cout << "1. Start sim\n2. Change start(" << startTime << ") stop(" << stopTime << ")\n3. Change interval(" << interval <<
		")\n4. Toggle interval/auto sim(";
	if (intervalMode)
		cout << "interval)";
	else
		cout << "auto)";

	cout << "\n5. Toggle verbose(" << m_verbose << ")\n6. Quit" << endl;

	return inputInt("Select: ", 1, 6);
}

//-----------------------------------------------------------------
// void UI::inputTimes(MyTime &startTime, MyTime &stopTime)
//
// Summary:	Lets the user enter start and stop time of visual feedback
//			Lets the user try again if they enter faulty values
//
// Parameters: MyTime &startTime - entered start time goes here
//				MyTime &stopTime - entered stop time goes here
//
// Returns:	-
//-----------------------------------------------------------------
void UI::inputTimes(MyTime &startTime, MyTime &stopTime)
{
	MyTime start(0), stop(0); //Tmp times so we can examine before applying

	cout << "Enter visual feedback start time (hh:mm): ";
	while (!(cin >> start))
	{
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n'); //Discard any data remaining in stream
	}
	cin.ignore(numeric_limits<streamsize>::max(), '\n'); //Discard any data remaining in stream

	cout << "Enter visual feedback stop time (hh:mm): ";
	while (!(cin >> stop))
	{
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n'); //Discard any data remaining in stream
	}
	cin.ignore(numeric_limits<streamsize>::max(), '\n'); //Discard any data remaining in stream

	if (start < stop)	//We dont want to have the start time be before or equal to stop time
	{
		startTime = start;
		stopTime = stop;
	}
	else
	{
		cout << "Error: Stop time cant be before start time!" << endl;
		waitForKeyboard();
	}
}

//-----------------------------------------------------------------
// void UI::displayEventLogs(const MyTime &from, const MyTime &to, const MyTime &stopTime, const std::vector<std::string> &logs)
//
// Summary:	Display event logs that took place within the start and stop time
//
// Parameters: 
//
// Returns:	-
//-----------------------------------------------------------------
void UI::displayEventLogs(const MyTime &from, const MyTime &to, const MyTime &stopTime, const std::vector<std::string> &logs)
{
	cout << "Events during " << from << " - ";
	if (to <= stopTime)
		cout << to;
	else
		cout << stopTime;
	cout << ":" << endl << endl;

	for (const string &log : logs)
		cout << log << endl;
}

//print simulation menu and return a user choice
int UI::simMenu()
{
	cout << "1. Continue sim\n2. Display schedule\n3. Train info\n4. Find wagon\n5. Wagon history\n6. Station info" << endl;
	return inputInt("Select: ", 1, 6);
}

//display train schedule
void UI::displaySchedule(const std::vector<std::weak_ptr<const Train>> schedule)
{
	cout << endl;

	for (int i = 0; i < schedule.size(); ++i)
	{
		auto train = schedule[i].lock();
		if (train)
		{
			cout << "Train " << train->getNumber() << " from " << train->getOriginName() << " " << train->getDeparture() << " to " << train->getDestinationName() << " " << train->getArrival() << endl;
		}
	}
}

//display information about a given train
void UI::displayTrainInfo(std::weak_ptr<const Train> train)
{
	cout << endl;
	auto trainPtr = train.lock();
	if (trainPtr)
	{
		cout << trainPtr->getInfo(m_verbose);
	}
	else
		cout << "That train does not exist" << endl;
}

//-----------------------------------------------------------------
// void UI::displayStationInfo(const std::vector<std::weak_ptr<const TrainStation>> stations) const
//
// Summary:	Prints the available stations and lets user select one.
//			User is the presented with information such as the trains
//			and wagons currently available at the station
//
// Parameters: const std::vector<std::weak_ptr<const TrainStation>> stations - selectable stations
//
// Returns:	-
//-----------------------------------------------------------------
void UI::displayStationInfo(const std::vector<std::weak_ptr<const TrainStation>> stations) const
{
	cout << endl;
	for (int i = 0; i < stations.size(); ++i)
	{
		auto currentStation = stations[i].lock();
		cout << i + 1 << ". " << currentStation->getName() << endl;
	}

	int sel = inputInt("Select: ", 1, stations.size());

	auto selectedStation = stations[sel-1].lock();

	std::vector<std::weak_ptr<const Train>> trains;
	selectedStation->getTrains(trains);
	cout << endl << "Stationed trains (" << trains.size() <<"):" << endl;
	for (auto train : trains)
	{
		auto trainPtr = train.lock();
		if (trainPtr)
			cout << trainPtr->getInfo(m_verbose) << endl;
	}

	multimap<WAGON_TYPE, std::weak_ptr<const TrainWagon>> wagons;
	selectedStation->getWagons(wagons);
	cout << "Available wagons (" << wagons.size() << "):" << endl;
	for (auto wagon : wagons)
	{
		const auto wagonPtr = wagon.second.lock();
		if (wagonPtr)
			cout << wagonPtr->getInfo(m_verbose) << endl;
	}
}

//Display the location of a wagon
void UI::displayWagonLocation(const int id, const string &location) const
{
	cout << endl;
	cout << "Wagon with id " << id;
	if (location.length() > 0)
		cout << " is currently located in " << location << endl;
	else
		cout << " does not exist" << endl;
}

//-----------------------------------------------------------------
// void UI::displayWagonHistory(const int id, const WagonHistory &history) const
//
// Summary:	Outputs the history of a wagon
//
// Parameters: const int id - id of the wagon which history is to be displayed
//			   const WagonHistory &history - contains a wagons history
//
// Returns:	-
//-----------------------------------------------------------------
void UI::displayWagonHistory(const int id, const WagonHistory &history) const
{
	cout << endl;
	cout << "History of wagon " << id << ":" << endl;
	for (const string &log : history.getLogs())
		cout << log << endl;
}

//-----------------------------------------------------------------
// void UI::displayStatistics
//
// Summary:	Shows some simulation statistics
//
// Parameters: const std::vector<std::shared_ptr<const Train>> &delayedTrains - the trains that were delayed
//			   const std::vector<std::shared_ptr<const Train>> &stuckTrains - the trains that never left the station
//			   const int numSuccessful - the number of trains that arrived on time
//			   const MyTime &totalDelay - total delay
//			   const MyTime &lastTrainTime - when the last train arrived at its destination
//
// Returns:	-
//-----------------------------------------------------------------
void UI::displayStatistics(const std::vector<std::shared_ptr<const Train>> &delayedTrains, const std::vector<std::shared_ptr<const Train>> &stuckTrains, const int numSuccessful, const MyTime &totalDelay, const MyTime &lastTrainTime)
{
	cout << "STATISTICS" << endl;
	cout << "Total delay: " << totalDelay << endl;
	cout << "Last train finished by: " << lastTrainTime + constants::ARRIVE_TO_FINISH_TIME << endl;
	cout << numSuccessful << " trains arrived in time" << endl << endl;

	cout << "Train that never left the station(" << stuckTrains.size() << ")" << endl;
	for (const auto train : stuckTrains)
		cout << train->getInfo(m_verbose) << endl;
		
	cout << endl << "Train that were delayed(" << delayedTrains.size() << ")" << endl;
	for (const auto train : delayedTrains)
	{
		cout << train->getArrivalDelay().getTime() << "minutes late: ";
		cout << "Train " << train->getNumber() << " from " << train->getOriginName() << " " << train->getDeparture() - train->getDepartureDelay() <<
			"(" <<train->getDeparture() << ") to " << train->getDestinationName() << " " << train->getArrival() - train->getArrivalDelay() << "(" << train->getArrival() << ")" << endl;
	}
}

//-----------------------------------------------------------------
// void waitForKeyboard()
//
// Summary:	Make user press enter to continue
//
// Parameters: -
//
// Returns:	-
//-----------------------------------------------------------------
void UI::waitForKeyboard() const
{
	cout << "Press enter to continue..";
	cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

//-----------------------------------------------------------------
// int TestApp::inputInt(const char* const prompt, const int &min, const int &max) const
//
// Summary:	Output message, get integer input between min and max from user
//			Default is min and max integer limits
//
// Parameters:	const char* const prompt - Tell user what to input
//				const int &min - Min accepted input value
//				const int &max - Max accepted input value
//
// Returns:	int - User input
//-----------------------------------------------------------------
int UI::inputInt(const char* const prompt, const int &min, const int &max) const
{
	int number;

	cout << prompt;
	while (!(cin >> number) || number < min || number > max)
	{
		cin.clear();	//Clear error flags
		cin.ignore(numeric_limits<streamsize>::max(), '\n'); //Discard any data remaining in stream
	}
	cin.ignore(numeric_limits<streamsize>::max(), '\n');

	return number;
}