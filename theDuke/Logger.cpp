#include "Logger.h"

void logger::log(move_t move) {
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