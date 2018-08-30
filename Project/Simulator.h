// File: Simulator.h
// Written by: Johan Lilja
// Date: 2015-10-25
// Last edited: 2015-11-01
// Course: DT060G OOP C++
// Assignment: Project Trains
#ifndef SIMULATOR_H
#define SIMULATOR_H
#include "Event.h"
#include "MyTime.h"
#include <queue>

class Company;
class UI;
class Simulator
{
private:
	Company *m_company;
	UI *m_ui;
	MyTime m_startTime;
	MyTime m_currentTime; // Time for last processed event
	const MyTime m_stopTime;
	MyTime m_visualStopTime;
	std::priority_queue<Event*, std::vector<Event*>, EventComparison> m_eventQueue;
	std::vector<std::string> m_eventLogs;
	bool m_intervalMode;
	int m_interval;

	void init();
	bool mainMenu();
	void simMenu() const;
	void statistics() const;
	void getSortedTrains(std::vector<std::weak_ptr<const Train>> &schedule) const;

public:
	Simulator(Company* const company, UI* const ui) : m_company(company), m_ui(ui), m_startTime(0), m_currentTime(0), m_visualStopTime("23:59"), m_stopTime("23:59"), m_eventQueue(), m_intervalMode(true), m_interval(10) { }
	void run();
	void scheduleEvent(Event *newEvent);
	void logEvent(const std::string &log) { m_eventLogs.push_back(log); }
	MyTime getTime() const { return m_currentTime; }
	MyTime getStopTime() const { return m_stopTime; }
	MyTime getVisualStopTime() const { return m_visualStopTime; }
	MyTime getStartTime() const { return m_startTime; }
	
};

#endif