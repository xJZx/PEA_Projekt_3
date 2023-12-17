#include <iostream>
#include <chrono>

/*Klasa mie��ca przebieg czasu w czasie wykonywania algorytmu*/
class Time
{
public:
	std::chrono::high_resolution_clock::time_point startTime;
	std::chrono::high_resolution_clock::time_point checkTime;

	Time();
	~Time();

	void start();
	void check();
	long long int totalTime();

};