#include "player.h"
#include <iostream>
#include <algorithm> 
#include <vector>
#include <string>
#include "figures.h"
/*	returns random name of figure which is in the pack
 * does NOT deploy -> for deployment is used function "deploy_troop"
*/
troop_name player_t::pick_random_backup_figure() {
	size_t size = packs.backup.size();

	if (size == 0) {
		return Nothing;
	}

	size_t index = rand() % size;
	std::vector<troop_name>::iterator it = packs.backup.begin();
	std::advance(it, index);
	troop_name new_unit = *it;
	return new_unit;
}

/** called by game::remove_figure()
*	removes troop from player's active troops (ergo killing it)
*	caller is responsible for removing the troop from the board - this function simply updates player's active pack
*/
void player_t::kill_troop(int x, int y) {
	for (auto it = packs.active.begin(); it != packs.active.end(); it++) {
		if (it->x == x && it->y == y) {
			packs.active.erase(it);
			break; //@todo: better
		}
	}
}

/* Just a big switch on troops name for printing active/backup pack
*/
void player_t::print_symbol(troop_name name) { //@todo: merge with log
	switch (name) {
	case Duke:
		std::cout << "Duke ";
		break;
	case Ranger: 
		std::cout << "Ranger ";
		break;
	case Assassin:
		std::cout << "Assassin ";
		break;
	case Pikeman:
		std::cout << "Pikeman ";
		break;
	case Marshall:
		std::cout << "Marshall ";
		break;
	case General:
		std::cout << "General ";
		break;
	case Knight:
		std::cout << "Knight ";
		break;
	case Longbowman:
		std::cout << "Longbowman ";
		break;
	case Champion:
		std::cout << "Champion ";
		break;
	case Wizard:
		std::cout << "Wizard ";
		break;
	case Seer:
		std::cout << "Seer ";
		break;
	case Footman:
		std::cout << "Footman ";
		break;
	case Priest:
		std::cout << "Priest ";
		break;
	case Bowman:
		std::cout << "Bowman ";
		break;
	case Dragoon:
		std::cout << "Dragoon ";
		break;
	default:
		std::cout << "Other ";
		break;
	}
}

/* As name suggests it prints names and coordinates of active troops*/
void player_t::print_active() {
	for (const auto& figure : packs.active) {
		print_symbol(figure.name);
		std::cout << "[" << figure.x << "," << figure.y << "] ";
	}
	std::cout << std::endl;
}

/* As name suggests it prints names (backup troops don't have coordinates) of backup troops*/
void player_t::print_backup() {
	for (const auto& figure : packs.backup) {
		print_symbol(figure);
	}
	std::cout << std::endl;
}

/* Called from game::move_troop, marks change of coordinates after move
	Because it's called after moving caller is responsible for checking legality of the move 	
*/
void player_t::change_coordinates(int fx, int fy, int tx, int ty) {
	for (auto& figure : packs.active) {
		if (figure.x == fx && figure.y == fy) {
			figure.x = tx;
			figure.y = ty; //@todo: remove and add again?
			break;
		}
	}
}

struct compare_troops
{
	inline bool operator()(const troop_t& first, const troop_t& second) {
		return first.name < second.name || (first.name == second.name && first.x < second.x) || (first.name == second.name && first.x == second.x && first.y < second.y);
	}
};
/*bool player::compare_troops(const troop& first,const troop& second) {
	return (first.name < second.name || (first.name == second.name && first.x < second.x) || (first.name == second.name && first.x == second.x && first.y < second.y));
}*/

bool compare_troops(const troop_t& first, const troop_t& second) {
	return (first.name < second.name || (first.name == second.name && first.x < second.x) || (first.name == second.name && first.x == second.x && first.y < second.y));
}

void player_t::sort_active_pack() {
	std::sort(packs.active.begin(), packs.active.end(),&compare_troops);
}

/* checks if the troop is available
* it is called by game::add_new_figure which checks for emptiness of square
* then it deployes troop - removes from backup pack and adds to active troops.
* caller is responsible for putting the troop on the board - this function simply updates player's packs
*/
bool packs_t::deploy_troop(troop_name figure, int x, int y) {
	if (x < 0 || x>5 || y < 0 || y>5) {
		return false;
	}

	auto iterator = std::find(backup.begin(), backup.end(), figure);
	if (iterator != backup.end()) {
		active.push_back(troop_t(x, y, *iterator));
		backup.erase(iterator);
		return true;
	}
	else {
		return false;
		std::cout << "not found";
	}
}

