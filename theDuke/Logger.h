#pragma once
#include <iostream>
#include <fstream>
#include <chrono>
#include <ctime>  
#include <stdio.h>
#include <time.h>
#include <string>
#include "Move.h"
#include "player.h"
#include <vector>

class logger {
public:
	logger() {
		even_move = false;
	}

	logger(std::string filename, std::vector<bool> first_chromozome, std::vector<bool> second_chromozome,parameters_t first_parameters, parameters_t second_parameters) {
		//file = std::ofstream(filename + ".txt");
		file.open(filename+".txt", std::ofstream::out | std::ofstream::trunc);
		file << "------------------------First---------------------" << std::endl;
		for(auto&& ch : first_chromozome){
			file << ch;
		}
		file << std::endl;
		for (auto&& item : first_parameters.troop_values) {
			file << item.second << ", ";
		}
		file << std::endl;
		file << "Coefficient duke moves " << first_parameters.coefficient_duke_moves << std::endl;
		file << "Coefficient moves " << first_parameters.coefficient_possible_moves << std::endl;
		file << "Coefficient active passive " << first_parameters.coeficient_active_passive << std::endl;
		file << "Adding take average " << !first_parameters.max_of_addition << std::endl;
		file << "------------------------Second---------------------" << std::endl;
		for (auto&& ch : second_chromozome) {
			file << ch;
		}
		file << std::endl;
		for (auto&& item : second_parameters.troop_values) {
			file << item.second << ", ";
		}
		file << std::endl;
		file << "Coefficient duke moves " << second_parameters.coefficient_duke_moves << std::endl;
		file << "Coefficient moves " << second_parameters.coefficient_possible_moves << std::endl;
		file << "Coefficient active passive " << second_parameters.coeficient_active_passive << std::endl;
		file << "Adding take average " << !second_parameters.max_of_addition << std::endl;
		file << "------------------------Game---------------------" << std::endl;
	}
	void create_file();
	void log(move_t move);
	void log_addition(coordinates coords, troop_name name);
	void log_message(std::string message);
	void update_branch_factor(long long situations, long long possibilities);
private:
	std::ofstream file;
	bool even_move;

};