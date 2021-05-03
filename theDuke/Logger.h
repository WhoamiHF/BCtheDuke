#pragma once
#include <iostream>
#include <fstream>
#include <chrono>
#include <ctime>  
#include <stdio.h>
#include <time.h>
#include <string>
#include "Move.h"

class logger {
public:
	logger() {
		std::time_t t = std::time(0);
		int xx = t;
		std::string s = std::to_string(xx);
		file = std::ofstream("Logs/" +s + ".txt");
		std::cout << xx;
		even_move = false;
	}

	void log(move_t move);
	void log_addition(coordinates coords, troop_name name);
private:
	std::ofstream file;
	bool even_move;

};