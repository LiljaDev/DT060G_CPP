// File: main.cpp
// Written by: Johan Lilja
// Date: 2015-10-25
// Last edited: 2015-11-01
// Course: DT060G OOP C++
// Assignment: Project Trains
#include <iostream>
#include <sstream>
#include <exception>
#include <stdexcept>
#include "Company.h"
#include "Simulator.h"
#include "UI.h"
using namespace std;

int run()
{
	UI ui;
	Company company;
	Simulator sim(&company, &ui);
	try
	{
		company.init();
		sim.run();
	}
	catch (ios_base::failure e)
	{
		cerr << "Error reading files" << endl << e.what();
		cin.ignore();
		return -1;
	}
	catch (runtime_error e)
	{
		cerr << e.what();
		cin.ignore();
		return -1;
	}
	catch (out_of_range e)
	{
		cerr << e.what();
		cin.ignore();
		return -1;
	}

	return 0;
}

int main()
{
	return run();
}