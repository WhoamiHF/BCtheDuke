#include "Logger.h"
#include <fstream>
#include <iostream>

void logger::create_file() {
	std::time_t t = std::time(0);
	int xx = t;
	std::string s = std::to_string(xx);
	file = std::ofstream("Logs/" + s + ".txt");
	std::cout << xx;
}
void logger::log_message(std::string message) {
	file << std::endl;
	file << "----------------Message-----------------" << std::endl;
	file << message << std::endl;
	file << "----------------End of message-------------------" << std::endl;
}

void logger::update_branch_factor(long long new_situations, long long new_possibilities) {
	std::ifstream branch_factor_file("Logs/branch_factor.txt");
	std::string line;
	std::getline(branch_factor_file, line);
	long long situations = stoi(line);
	std::getline(branch_factor_file, line);
	long long possibilities = stoi(line);
	branch_factor_file.close();

	std::ofstream ofs;
	ofs.open("Logs/branch_factor.txt", std::ofstream::out | std::ofstream::trunc);
	ofs << situations + new_situations << std::endl;
	ofs << possibilities + new_possibilities << std::endl;
	ofs.close();
}

void logger::log(move_t move) {
	if (!file.is_open()) {
		create_file();
	}
	switch (move.op) {
	case add_it:
		//special function for loggin adition
		break;
	case move_it:
		file << "Move [" << move.coords[0].x << "," << move.coords[0].y << "] to [" << move.coords[1].x << "," << move.coords[1].y << "] ";
		break;
	case command_it:
		file << "Command [" << move.coords[0].x << "," << move.coords[0].y  << "] " << move.coords[1].x << "," << move.coords[1].y << "] to [" << move.coords[2].x << "," << move.coords[2].y << "] ";
		break;
	default:
		break;
	}
	if (even_move) {
		file << std::endl;
	}
	even_move = !even_move;
}

void logger::log_addition(coordinates coords, troop_name type) {
	if (!file.is_open()) {
		create_file();
	}
	std::string name = "";
	switch (type) {
	case Duke:
		name = "Duke";
		break;
	case Pikeman:
		name = "Pikeman";
		break;
	case Marshall:
		name = "Marshall";
		break;
	case Ranger:
		name = "Ranger";
		break;
	case Assassin:
		name = "Assassin";
		break;
	case General:
		name = "General";
		break;
	case Knight:
		name = "Knight";
		break;
	case Longbowman:
		name = "Longbowman";
		break;
	case Champion:
		name = "Champion";
		break;
	case Wizard:
		name = "Wizard";
		break;
	case Seer:
		name = "Seer";
		break;
	case Footman:
		name = "Footman";
		break;
	case Priest:
		name = "Priest";
		break;
	case Bowman:
		name = "Bowman";
		break;
	case Dragoon:
		name = "Dragoon";
		break;
	default:
		name = "unknown";
		break;
	}
	file << "Add "<< name <<"[" << coords.x << "," << coords.y << "] ";
	if (even_move) {
		file << std::endl;
	}
	even_move = !even_move;
}