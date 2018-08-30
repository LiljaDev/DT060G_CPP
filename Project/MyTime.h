// File: MyTime.h
// Written by: Johan Lilja
// Date: 2015-10-25
// Last edited: 2015-11-01
// Course: DT060G OOP C++
// Assignment: Project Trains
#ifndef MYTIME_H
#define MYTIME_H
#include <string>
#include <ostream>
class MyTime
{
private:
	int m_time;
public:
	MyTime(const std::string &time);
	MyTime(const int time) { m_time = time; }
	int getTime() const { return m_time; }
	//void getTime(std::string &time) const;

	MyTime& operator+=(const int rhs){ m_time += rhs; return *this; }
	friend MyTime operator+(MyTime lhs, const int rhs) { lhs += rhs; return lhs; }
	friend MyTime operator-(const MyTime &lhs, const int rhs) { MyTime result(lhs.m_time-rhs); return result; }
	friend MyTime operator-(const MyTime &lhs, const MyTime &rhs) { return lhs - rhs.m_time; }

	friend bool operator<(const MyTime &lhs, const MyTime &rhs) { return lhs.m_time < rhs.m_time; }
	friend bool operator<(const MyTime &lhs, const int rhs) { return lhs.m_time < rhs; }
	friend bool operator<=(const MyTime &lhs, const MyTime &rhs) { return !(lhs > rhs); }

	friend bool operator>(const MyTime &lhs, const MyTime &rhs) { return rhs.m_time < lhs.m_time; }
	friend bool operator>(const MyTime &lhs, const int rhs) { return lhs.m_time > rhs; }
	friend bool operator>=(const MyTime &lhs, const MyTime &rhs) { return !(lhs < rhs); }

	friend bool operator==(const MyTime &lhs, const MyTime &rhs) { return lhs.m_time == rhs.m_time; }
	friend std::istream &operator>>(std::istream &is, MyTime &time);
};

std::ostream &operator<<(std::ostream &os, const MyTime &time);

#endif