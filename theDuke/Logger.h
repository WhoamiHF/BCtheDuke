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
		even_move = false;
	}
	void create_file();
	void log(move_t move);
	void log_addition(coordinates coords, troop_name name);
private:
	std::ofstream file;
	bool even_move;

};