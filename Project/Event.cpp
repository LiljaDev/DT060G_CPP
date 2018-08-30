// File: Event.cpp
// Written by: Johan Lilja
// Date: 2015-10-25
// Last edited: 2015-11-01
// Course: DT060G OOP C++
// Assignment: Project Trains
#include "Event.h"
#include "Company.h"
#include "Simulator.h"
#include "Train.h" //NOT SO NICE PUT STATE IN CONSTANTS
#include "Constants.h"
using namespace std;
using namespace constants;

//-----------------------------------------------------------------
// void AssembleEvent::processEvent()
//
// Summary: Tries to assemble train based on required wagons
//			by checking if there are wagons available at the station.
//			Either an assembly or a ready event is scheduled based on assembly success
//			Function aborts if the simulation stop time has been exceeded
//			Writes to a log if the event time is within the visual feedback limits
//
// Parameters:	-
//
// Returns:	-
//-----------------------------------------------------------------
void AssembleEvent::processEvent()
{
	const shared_ptr<const Train> train = m_train.lock();
	if (!train || m_time > m_sim->getStopTime())	// aborts if the simulation stop time has been exceeded or if train has expired
		return;
	
	TRAIN_STATE oldState = train->getState();
	TRAIN_STATE newState = m_company->assembleTrain(train->getNumber());	//Try to assemble train

	Event *newEvent = nullptr;
	if (newState == INCOMPLETE)	//assemble failed
	{
		m_company->delayTrain(train->getNumber(), RETRY_ASSEMBLE_WAIT);
		newEvent = new AssembleEvent(m_sim, m_company, train->getDeparture() - ASSEMBLE_TO_RUN_TIME, m_train);
	}
	else if (newState == ASSEMBLED)	//assemble succeeded
	{
		m_company->delayTrain(train->getNumber(), 0);	//fix: recalc arrival since last wagon could have been a locomotive with lower max speed
		newEvent = new ReadyEvent(m_sim, m_company, train->getDeparture() - READY_TO_RUN_TIME, m_train);
	}

	m_sim->scheduleEvent(newEvent);

	if (newState != oldState && m_time >= m_sim->getStartTime() && m_time <= m_sim->getVisualStopTime())	//log event if there was a state change and time is within visual feedback interval
	{
		ostringstream log;
		log << "(" << m_time << ") Train: " << train->getNumber() << " is now " << TRAIN_STATE_NAMES[train->getState()] << endl;
		m_sim->logEvent(log.str());
	}
}

//-----------------------------------------------------------------
// void ReadyEvent::processEvent()
//
// Summary: Sets train state to ready and scedules a run event
//			Function aborts if the simulation stop time have been exceeded
//			Writes to a log if the event time is within the visual feedback limits
//
// Parameters:	-
//
// Returns:	-
//-----------------------------------------------------------------
void ReadyEvent::processEvent()
{
	const shared_ptr<const Train> train = m_train.lock();
	if (!train || m_time > m_sim->getStopTime())	//aborts if the simulation stop time have been exceeded or if train has expired
		return;

	m_company->readyTrain(train->getNumber());

	Event *newEvent = new RunEvent(m_sim, m_company, train->getDeparture(), m_train);
	m_sim->scheduleEvent(newEvent);

	if (m_time >= m_sim->getStartTime() && m_time <= m_sim->getVisualStopTime())	//log event if time is within visual feedback interval
	{
		ostringstream log;
		log << "(" << m_time << ") Train: " << train->getNumber() << " is now " << TRAIN_STATE_NAMES[READY] << endl;
		m_sim->logEvent(log.str());
	}
}

//-----------------------------------------------------------------
// void RunEvent::processEvent()
//
// Summary: Train goes into run state and a arrive event is sceduled
//			Function aborts if the simulation stop time have been exceeded
//			Writes to a log if the event time is within the visual feedback limits
//
// Parameters:	-
//
// Returns:	-
//-----------------------------------------------------------------
void RunEvent::processEvent()
{
	const shared_ptr<const Train> train = m_train.lock();
	if (!train || m_time > m_sim->getStopTime())	//aborts if the simulation stop time have been exceeded or if train has expired
		return;

	m_company->runTrain(train->getNumber());

	Event *newEvent = new ArriveEvent(m_sim, m_company, train->getArrival(), m_train);
	m_sim->scheduleEvent(newEvent);

	if (m_time >= m_sim->getStartTime() && m_time <= m_sim->getVisualStopTime())	//log event if time is within visual feedback interval
	{
		ostringstream log;
		log << "(" << m_time << ") Train: " << train->getNumber() << " is now " << TRAIN_STATE_NAMES[RUNNING] << endl;
		m_sim->logEvent(log.str());
	}
}

//-----------------------------------------------------------------
// void ArriveEvent::processEvent()
//
// Summary: Train goes into arrived state and a disassemble event is sceduled
//			Writes to a log if the event time is within the visual feedback limits
//
// Parameters:	-
//
// Returns:	-
//-----------------------------------------------------------------
void ArriveEvent::processEvent()
{
	const shared_ptr<const Train> train = m_train.lock();
	if (!train) //Abort if train has expired
		return;

	m_company->arriveTrain(train->getNumber());
	Event *newEvent = new DisassembleEvent(m_sim, m_company, train->getArrival() + ARRIVE_TO_FINISH_TIME, m_train);
	m_sim->scheduleEvent(newEvent);

	if (m_time >= m_sim->getStartTime() && m_time <= m_sim->getVisualStopTime())	//log event if time is within visual feedback interval
	{
		ostringstream log;
		log << "(" << m_time << ") Train: " << train->getNumber() << " is now " << TRAIN_STATE_NAMES[ARRIVED] << endl;
		m_sim->logEvent(log.str());
	}
}

//-----------------------------------------------------------------
// void DisassembleEvent::processEvent()
//
// Summary: Train goes into finished state
//			The wagons are disconnected and transfered to the station wagon pool
//			Writes to a log if the event time is within the visual feedback limits
//
// Parameters:	-
//
// Returns:	-
//-----------------------------------------------------------------
void DisassembleEvent::processEvent()
{
	const shared_ptr<const Train> train = m_train.lock();
	if (!train)	//Abort if train has expired
		return;

	m_company->disassembleTrain(train->getNumber());

	if (m_time >= m_sim->getStartTime() && m_time <= m_sim->getVisualStopTime())	//log event if time is within visual feedback interval
	{
		ostringstream log;
		log << "(" << m_time << ") Train: " << train->getNumber() << " is now " << TRAIN_STATE_NAMES[FINISHED] << endl;
		m_sim->logEvent(log.str());
	}
}