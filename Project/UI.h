// File: UI.h
// Written by: Johan Lilja
// Date: 2015-10-25
// Last edited: 2015-11-01
// Course: DT060G OOP C++
// Assignment: Project Trains
#ifndef UI_H
#define UI_H
#include "MyTime.h"
#include "WagonHistory.h"
#include <limits>
#include <vector>
#include <memory>

class Train;
class TrainStation;
class UI
{
private:
	bool m_verbose;
public:
	UI() : m_verbose(true) {}
	int mainMenu(const MyTime &startTime, const MyTime &stopTime, const int interval, const bool intervalmode);
	int simMenu();
	void displaySchedule(const std::vector<std::weak_ptr<const Train>> schedule);
	void displayTrainInfo(std::weak_ptr<const Train> train);
	void displayWagonLocation(const int id, const std::string &location) const;
	void displayWagonHistory(const int id, const WagonHistory &history) const;
	void displayStationInfo(const std::vector<std::weak_ptr<const TrainStation>> stations) const;
	void displayStatistics(const std::vector<std::shared_ptr<const Train>> &delayedTrains, const std::vector<std::shared_ptr<const Train>> &stuckTrains, const int numSuccessful, const MyTime &totalDelay, const MyTime &lastTrainTime);
	void inputTimes(MyTime &startTime, MyTime &stopTime);
	void toggleVerbose() { m_verbose = !m_verbose; }
	void displayEventLogs(const MyTime &from, const MyTime &to, const MyTime &stopTime, const std::vector<std::string> &logs);
	void clearScreen() const { system("cls"); }
	void waitForKeyboard() const;	//Make user press enter to continue
	int inputInt(const char* const prompt, const int &min = std::numeric_limits<int>::min(), const int &max = std::numeric_limits<int>::max()) const;	//Output message, get integer input between min and max from user
};

#endif