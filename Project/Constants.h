// File: Constants.h
// Written by: Johan Lilja
// Date: 2015-10-25
// Last edited: 2015-11-01
// Course: DT060G OOP C++
// Assignment: Project Trains
#ifndef CONSTANTS_H
#define CONSTANTS_H

namespace constants{
	const char* const STATIONS_FILENAME = "TrainStations.txt";
	const char* const MAP_FILENAME = "TrainMap.txt";
	const char* const TRAINS_FILENAME = "Trains.txt";
	const char STATIONS_DELIM = ')';
	const int WAGON_TYPE_COUNT = 6;
	const int ASSEMBLE_TO_RUN_TIME = 30;
	const int RETRY_ASSEMBLE_WAIT = 10;
	const int READY_TO_RUN_TIME = 10;
	const int ARRIVE_TO_FINISH_TIME = 20;
}

#endif