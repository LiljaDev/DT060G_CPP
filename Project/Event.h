// File: Event.h
// Written by: Johan Lilja
// Date: 2015-10-25
// Last edited: 2015-11-01
// Course: DT060G OOP C++
// Assignment: Project Trains

#ifndef EVENT_H
#define EVENT_H
#include "MyTime.h"
#include <memory>
// Forward declarations
class Simulator;
class Company;
class Train;
class Event {
public:
	// Constructor requires time of event
	Event(Simulator *sim, Company *company, const MyTime &t) : m_sim(sim), m_company(company), m_time(t) { }

	virtual ~Event() { }

	// Process event by invoking this method
	virtual void processEvent() = 0;

	// Get time for this event
	MyTime getTime() const {
		return m_time;
	}

protected:
	// Time for this event
	MyTime m_time;
	Simulator *m_sim;
	Company *m_company;
};



// Used to compare to Events with respect to time
class EventComparison {
public:
	bool operator() (Event * left, Event * right) {
		return left->getTime() > right->getTime();
	}
};

//--- Derived Event-classes ----------------------------------------------------

class AssembleEvent : public Event {
public:
	AssembleEvent(Simulator *sim, Company *company, MyTime time, const std::weak_ptr<const Train> train)
		: Event(sim, company, time), m_train(train) { }

	virtual void processEvent();

protected:
	const std::weak_ptr<const Train> m_train;
};

class ReadyEvent : public Event {
public:
	ReadyEvent(Simulator *sim, Company *company, MyTime time, const std::weak_ptr<const Train> train)
		: Event(sim, company, time), m_train(train) { }

	virtual void processEvent();

protected:
	const std::weak_ptr<const Train> m_train;
};

class RunEvent : public Event {
public:
	RunEvent(Simulator *sim, Company *company, MyTime time, const std::weak_ptr<const Train> train)
		: Event(sim, company, time), m_train(train) { }

	virtual void processEvent();

protected:
	const std::weak_ptr<const Train> m_train;
};

class ArriveEvent : public Event {
public:
	ArriveEvent(Simulator *sim, Company *company, MyTime time, const std::weak_ptr<const Train> train)
		: Event(sim, company, time), m_train(train) { }

	virtual void processEvent();

protected:
	const std::weak_ptr<const Train> m_train;
};

class DisassembleEvent : public Event {
public:
	DisassembleEvent(Simulator *sim, Company *company, MyTime time, const std::weak_ptr<const Train> train)
		:Event(sim, company, time), m_train(train) { }

	virtual void processEvent();

protected:
	const std::weak_ptr<const Train> m_train;
};

#endif